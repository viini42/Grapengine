add_executable(Game Game.cpp Game.hpp
    MainLayer.cpp
    MainLayer.hpp
    SpaceLevel.cpp
    SpaceLevel.hpp
    ParticlesSystem.cpp
    ParticlesSystem.hpp)

target_link_libraries(Game PRIVATE Grapengine)
target_precompile_headers(Game PRIVATE game_pch.hpp)

