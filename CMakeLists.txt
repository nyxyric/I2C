# copied and adjusted from https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/
cmake_minimum_required(VERSION 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(2048)
find_package(SDL2 REQUIRED)

# link SDL2 TTF cmake to make TTF functions avaiable
include(${PROJECT_SOURCE_DIR}/cmake/FindSDL2TTF.cmake)

# include the header files
include_directories(${PROJECT_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

# create the executable based on our c files
add_executable(2048 src/core.c src/game.c)

# link the SDL2 and TTF libraries to the executable created before
target_link_libraries(2048 -lm ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY} )

# copy the font file into the bin folder    
FILE(COPY ${CMAKE_SOURCE_DIR}/res/SansSemiBold.ttf DESTINATION "${CMAKE_BINARY_DIR}/bin")


