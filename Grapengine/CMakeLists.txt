set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# #############################################################################
# Engine
file(GLOB_RECURSE GRAPES_SOURCES
  controllers/*.cpp
  core/*.cpp
  drawables/*.cpp
  events/*.cpp
  input/*.cpp
  serializer/*.cpp
  layer/*.cpp
  log/*.cpp
  math/*.cpp
  profiling/*.cpp
  renderer/*.cpp
  scene/*.cpp
  utils/*.cpp
)

file(GLOB_RECURSE GRAPES_HEADERS
  controllers/*.hpp
  core/*.hpp
  drawables/*.hpp
  events/*.hpp
  input/*.hpp
  serializer/*.hpp
  layer/*.hpp
  log/*.hpp
  math/*.hpp
  profiling/*.hpp
  renderer/*.hpp
  scene/*.hpp
  utils/*.hpp
)

add_library(Grapengine STATIC ${GRAPES_HEADERS} ${GRAPES_SOURCES})

if(DEFINED USE_TRACY_PROFILER)
  message(STATUS "GRAPENGINE: Enabling tracy profiler")
  target_compile_definitions(Grapengine PUBLIC USE_TRACY_PROFILER TRACY_ENABLE TRACY_NO_SYSTEM_TRACING)
  target_include_directories(Grapengine SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/vendor/tracy)
endif()

target_precompile_headers(Grapengine PRIVATE ${CMAKE_SOURCE_DIR}/Grapengine/grapengine_pch.hpp)
target_include_directories(Grapengine
  PUBLIC ${CMAKE_SOURCE_DIR}/Grapengine
)

target_compile_definitions(Grapengine PRIVATE
  GLFW_INCLUDE_NONE
  GRAPENGINE_CORE
)

target_link_libraries(Grapengine PUBLIC GrapengineCompileOptions)

##############################################################################
#
if(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
  add_custom_target(
    GrapeTidy
    DEPENDS Grapengine
    COMMAND clang-tidy --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy -p ${CMAKE_BINARY_DIR} --quiet --export-fixes=${CMAKE_SOURCE_DIR}/fixes.yaml --header-filter='^ge_.*?\.hpp$$'
    ${GRAPES_SOURCES}
  )
  add_custom_target(
    GrapeTidyWithClangD
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/Grapengine/RunClangd.cmake "${CMAKE_BINARY_DIR}" "${GRAPES_SOURCES}"
    DEPENDS Grapengine
  )
endif()

# ############################################################################
# imgui
file(GLOB IMGUI_SRC ${CMAKE_SOURCE_DIR}/vendor/imgui/*.cpp ${CMAKE_SOURCE_DIR}/imgui/*.h)
add_library(ImGui STATIC
  ${IMGUI_SRC}
  ${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
  ${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.h
  ${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
  ${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.h
  ${CMAKE_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.h
  ${CMAKE_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp
)
if(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
  target_compile_options(ImGui PRIVATE -w)
endif()
set_target_properties(
  ImGui
  PROPERTIES
  CXX_CLANG_TIDY ""
)
target_include_directories(ImGui SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui)
target_link_libraries(Grapengine PUBLIC ImGui)

# #############################################################################
# Dependencies from VCPKG
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Grapengine PRIVATE glfw)
target_link_libraries(ImGui PRIVATE glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(Grapengine PRIVATE glad::glad)

find_package(Stb REQUIRED)
target_include_directories(Grapengine SYSTEM PRIVATE ${Stb_INCLUDE_DIR})

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(Grapengine PUBLIC spdlog::spdlog_header_only)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(Grapengine PRIVATE yaml-cpp::yaml-cpp)
