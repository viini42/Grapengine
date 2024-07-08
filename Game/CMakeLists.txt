add_executable(Game Game.cpp Game.hpp
    MainLayer.cpp
    MainLayer.hpp)

target_link_libraries(Game PRIVATE Grapengine)
target_precompile_headers(Game PRIVATE game_pch.hpp)

add_custom_command(
    TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Making assets directory"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E echo "Copying assets files"
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Game/assets/ ${CMAKE_CURRENT_BINARY_DIR}/assets
)
