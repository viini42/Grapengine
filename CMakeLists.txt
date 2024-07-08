cmake_minimum_required(VERSION 3.22)

if (NOT DEFINED ${CMAKE_TOOLCHAIN_FILE})
  if (CMAKE_HOST_SYSTEM_NAME STREQUAL Windows)
    set(VCPKG_TARGET_TRIPLET x64-windows-static)
  elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL Linux)
    set(VCPKG_TARGET_TRIPLET x64-linux)
  else ()
    message(FATAL_ERROR "GRAPENGINE: Unsupported platform")
  endif ()

  set(VCPKG_INSTALL_OPTIONS "--no-print-usage")

  message(STATUS "GRAPENGINE: Defining vcpkg as toolchain file")
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake)
  message(STATUS "GRAPENGINE: CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif ()

project(Grapengine CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
  if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always -fansi-escape-codes)
  endif ()
endif ()

include(CompilationOptions.cmake)

# #############################################################################
# Engine
add_subdirectory(Grapengine)

# #############################################################################
# App
add_subdirectory(Wineglass)

# #############################################################################
# Tests
add_subdirectory(tests)

# #############################################################################
# Scripts
add_subdirectory(Wineglass/nativescripts)

add_subdirectory(Game)