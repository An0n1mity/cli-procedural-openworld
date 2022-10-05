cmake_minimum_required(VERSION 3.0.0)
project(CProject VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(CProject main.c Block.c Entity.c Player.c Entity.c Tilemap.c PerlinNoise.c Rendering.c Menu.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_link_libraries(CProject ${CURSES_LIBRARIES} m)

include(CPack)
