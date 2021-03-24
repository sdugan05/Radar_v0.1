cmake_minimum_required(VERSION 3.17)
project(Radar_v0_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Radar_v0_1 main.cpp utils/process/process.cpp utils/Vector2.cpp utils/Vector2.hpp Weapons.hpp Weapons.cpp Offsets.cpp Offsets.hpp Player.cpp Player.hpp)