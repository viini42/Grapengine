# Tests Codex Analysis

## Current state

I ran a clean rebuild of the test target and inspected both the test CMake wiring and the current test sources.

Build result:

```text
cmake --build build-clang-linux-debug --target EngineTests --clean-first
```

This completed successfully.

CTest result:

```text
ctest --test-dir build-clang-linux-debug -N
Total Tests: 0
```

This means the test executable exists, but the test project is not integrated with CTest execution.

Direct execution result:

```text
./build-clang-linux-debug/tests/EngineTests
```

This starts correctly and passes the pure logic tests, but aborts on the first GLFW/OpenGL-dependent test when `glfwInit()` fails in the current environment.

## Main issues

### 1. Tests are not registered with CTest

File: [tests/CMakeLists.txt](/home/vini/Projects/Grapengine/tests/CMakeLists.txt)

The file enables testing and builds `EngineTests`, but it never calls:

- `add_test(...)`
- `gtest_discover_tests(...)`
- `include(GoogleTest)`

Because of that, `ctest` sees zero tests. This is the biggest structural reason the test project has gone stale: it is not part of the normal test workflow.

### 2. Unit tests and integration tests are mixed in one binary

Examples:

- Pure logic tests:
  - `tests/test_vector.cpp`
  - `tests/test_transformations.cpp`
  - `tests/test_events.cpp`
  - `tests/test_random.cpp`
  - `tests/test_components_serializer.cpp`

- Context-dependent tests:
  - [tests/test_window.cpp](/home/vini/Projects/Grapengine/tests/test_window.cpp)
  - [tests/test_shader.cpp](/home/vini/Projects/Grapengine/tests/test_shader.cpp)
  - [tests/test_vertex_array.cpp](/home/vini/Projects/Grapengine/tests/test_vertex_array.cpp)

The problem is that the graphics tests require a real GLFW/OpenGL environment, while the math and serializer tests are fully headless.

When all of them live in one binary, one early environment failure stops useful feedback from the rest of the suite.

### 3. GLFW/OpenGL-dependent tests are not environment-aware

The first hard runtime failure happens in [tests/test_shader.cpp](/home/vini/Projects/Grapengine/tests/test_shader.cpp:68), which creates a real `Window`.

That reaches [Grapengine/core/ge_window.cpp](/home/vini/Projects/Grapengine/Grapengine/core/ge_window.cpp:24):

```cpp
const bool success = bool(glfwInit());
GE_ASSERT_OR_RETURN_VOID(success, "Failed to initialize GLFW");
```

In the current environment, `glfwInit()` fails, so the test binary aborts before the graphics tests can produce meaningful pass/fail results.

### 4. Some tests assert weak success conditions

Example: [tests/test_window.cpp](/home/vini/Projects/Grapengine/tests/test_window.cpp:12)

It checks that `MakeScope<Window>` returns a non-null pointer, but that does not prove the underlying GLFW window/context was initialized successfully. The constructor can return early after an assertion path while the object still exists.

That makes the test weaker than it appears.

### 5. Test CMake has stale or dead configuration

File: [tests/CMakeLists.txt](/home/vini/Projects/Grapengine/tests/CMakeLists.txt:12)

```cmake
target_include_directories(EngineTests PRIVATE ${ENGINE_INCLUDE})
```

`ENGINE_INCLUDE` is not defined anywhere relevant in the project. The target still builds because `Grapengine` exports its include directories transitively.

This line is effectively dead configuration and a sign the test target has drifted.

### 6. Clean rebuilds are noisy and heavier than needed

A clean rebuild of only `EngineTests` still rebuilds the full `Grapengine` library and the vendored `ImGui` target. Most of the warning output comes from vendored ImGui, not from your tests.

That creates two practical problems:

- test feedback is noisy
- rebuilds are slower than they need to be

This kind of friction is one of the main reasons test suites stop being used regularly.

### 7. Many tests rely on death-test assertion behavior

Examples:

- [tests/test_scene.cpp](/home/vini/Projects/Grapengine/tests/test_scene.cpp:28)
- [tests/test_io.cpp](/home/vini/Projects/Grapengine/tests/test_io.cpp:13)
- [tests/test_vertex_array.cpp](/home/vini/Projects/Grapengine/tests/test_vertex_array.cpp:17)

These are valid in principle, but they also generate repeated Clang warnings from GTest internals. That adds more noise and makes failures harder to scan quickly.

## What is working

Even with the current problems, the underlying test code is not completely broken.

From direct execution of `EngineTests`, these categories are still useful and passing before the graphics/environment failure:

- camera math
- component serialization basics
- event behavior
- IO assertions
- random helpers
- scene basics
- scene serialization

That means there is a useful core suite already present. The problem is mostly organization, execution model, and environment coupling.

## Recommended step-by-step guide to organize the tests project

### Step 1. Split tests into two categories

Create two test targets instead of one:

- `EngineUnitTests`
  - pure logic
  - no GLFW
  - no OpenGL context
  - safe for CI and headless environments

- `EngineGraphicsTests`
  - creates `Window`
  - depends on GLFW/OpenGL
  - runs only in environments with a display/context

Move files roughly like this:

- `EngineUnitTests`
  - `test_vector.cpp`
  - `test_transformations.cpp`
  - `test_events.cpp`
  - `test_random.cpp`
  - `test_io.cpp`
  - `test_scene.cpp`
  - `test_scene_serializer.cpp`
  - `test_components_serializer.cpp`
  - `test_camera.cpp`

- `EngineGraphicsTests`
  - `test_window.cpp`
  - `test_shader.cpp`
  - `test_vertex_array.cpp`

### Step 2. Register tests with CTest

In `tests/CMakeLists.txt`, add proper discovery or explicit registration.

Preferred approach:

```cmake
include(GoogleTest)
gtest_discover_tests(EngineUnitTests)
gtest_discover_tests(EngineGraphicsTests)
```

If you want simpler control at first:

```cmake
add_test(NAME EngineUnitTests COMMAND EngineUnitTests)
add_test(NAME EngineGraphicsTests COMMAND EngineGraphicsTests)
```

This fixes the biggest current problem: `ctest` must become the standard way to run the suite.

### Step 3. Make graphics tests skippable or gated

Before running context-dependent tests, check whether GLFW initialization is available.

You have two reasonable options:

1. Add a tiny helper that attempts `glfwInit()` and skips the test with `GTEST_SKIP()` if unavailable.
2. Exclude `EngineGraphicsTests` from CI/headless runs and only run them in environments where graphics are available.

This prevents the current situation where one environment-dependent failure blocks the whole test binary.

### Step 4. Remove stale CMake from the tests target

Delete or replace:

```cmake
target_include_directories(EngineTests PRIVATE ${ENGINE_INCLUDE})
```

Rely on:

```cmake
target_link_libraries(... PRIVATE Grapengine)
```

unless you explicitly want extra include paths for a separate reason.

### Step 5. Reduce third-party warning noise

The clean rebuild log is dominated by warnings from vendored ImGui.

You should avoid letting third-party warning spam drown out test feedback. Reasonable options:

- keep vendored targets built as `SYSTEM` includes
- relax warnings for third-party targets only
- avoid rebuilding ImGui when you only want headless unit tests

The goal is that a test rebuild should mostly show your test/build problems, not vendor internals.

### Step 6. Strengthen the graphics tests

Tests like [tests/test_window.cpp](/home/vini/Projects/Grapengine/tests/test_window.cpp:12) should verify stronger postconditions than “pointer is not null”.

Examples:

- window native handle is valid
- created dimensions are preserved
- OpenGL context initialization succeeded
- expected renderer object reports valid state

This makes the tests meaningful instead of just smoke-level construction checks.

### Step 7. Revisit death tests

Keep death tests where they are truly valuable, but do not overuse them for ordinary error handling.

Questions to ask for each one:

- Is the code supposed to abort in debug builds?
- Is this really public contract behavior?
- Would a normal error-return or exception test be more stable?

This will reduce noise and make the suite more intentional.

### Step 8. Add one fast default path and one extended path

Define two standard workflows:

Fast default:

```text
ctest -L unit
```

Extended or graphics-enabled:

```text
ctest -L graphics
```

You can label tests when you register them. This gives you:

- a fast, always-runnable suite
- a larger environment-dependent suite

That separation is one of the most effective ways to keep tests maintained.

### Step 9. Make test health visible

Once CTest is wired up, make these commands the normal ones:

```sh
cmake --build build-clang-linux-debug --target EngineUnitTests
ctest --test-dir build-clang-linux-debug --output-on-failure
```

If developers can run tests the same way every time, the suite is much less likely to stall again.

## Suggested execution order for cleanup

1. Register the existing test executable with CTest.
2. Split headless tests from graphics tests.
3. Gate graphics tests with `GTEST_SKIP()` or a separate target.
4. Remove stale CMake config like `${ENGINE_INCLUDE}`.
5. Tighten weak tests.
6. Reduce vendor warning noise.

## Bottom line

The tests project is not dead, but it is operationally broken:

- it builds
- a useful subset of tests still passes
- it is not discoverable by CTest
- it is not organized by runtime requirements
- one graphics/environment failure can stop the whole run

The right next move is not rewriting all tests. It is reorganizing execution, target boundaries, and registration so the existing suite becomes runnable and trustworthy again.
