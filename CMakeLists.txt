cmake_minimum_required(VERSION 3.0.0)
project(CProject VERSION 0.1.0)

include(CTest)
enable_testing()
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(CProject main.c Block.c Entity.c Player.c Entity.c Tilemap.c PerlinNoise.c Rendering.c Menu.c Chunk.c Chicken.c Camera.c Inventory.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_link_libraries(CProject ncursesw m form menu)

include(CPack)