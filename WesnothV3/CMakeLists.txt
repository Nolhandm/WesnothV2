cmake_minimum_required(VERSION 3.24)
project(WesnothV3)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(WesnothV3 main.cpp Game.cpp Game.hpp Types.hpp Utilitaire.cpp Utilitaire.hpp MCTS.cpp MCTS.hpp Node.cpp Node.hpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(c:/SFML/include)
target_link_libraries(WesnothV3 sfml-system sfml-window sfml-graphics sfml-audio)