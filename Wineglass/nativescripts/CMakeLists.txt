# #############################################################################
# Scripts
add_library(NativeScripts STATIC
  CamController.hpp
  CamController.cpp
)

target_precompile_headers(NativeScripts PRIVATE NativeScriptsPCH.hpp)

target_include_directories(NativeScripts SYSTEM PRIVATE
  ${CMAKE_SOURCE_DIR}/Grapengine
)
target_link_libraries(NativeScripts PRIVATE Grapengine)
