# #############################################################################
# Wineglass
add_executable(Wine
  Editor.cpp
  Editor.hpp
  EditorLayer.cpp
  EditorLayer.hpp
  panels/SceneHierarchyPanel.cpp
  panels/SceneHierarchyPanel.hpp
)

target_precompile_headers(Wine PRIVATE WineglassPCH.hpp)

#add_executable(Wine main.cpp)
target_include_directories(Wine SYSTEM PRIVATE
  ${CMAKE_SOURCE_DIR}/Grapengine
  ${CMAKE_SOURCE_DIR}/vendor/imgui
)
target_link_libraries(Wine PRIVATE Grapengine)
target_link_libraries(Wine PRIVATE NativeScripts)

add_custom_command(
  TARGET Wine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Making assets directory"
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/Assets
  COMMAND ${CMAKE_COMMAND} -E echo "Copying assets files"
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets/ ${CMAKE_CURRENT_BINARY_DIR}/Assets
  COMMAND ${CMAKE_COMMAND} -E echo "Copying imgui configuration"
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Wineglass/imgui.ini ${CMAKE_CURRENT_BINARY_DIR}/imgui.ini
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Wineglass/imgui.ini ${CMAKE_SOURCE_DIR}/imgui.ini
)
