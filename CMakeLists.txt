cmake_minimum_required(VERSION 3.31)
project(flappy_bird)

add_subdirectory(caffeine-gl)

add_executable(flappy_bird
    src/main.cpp
)

target_compile_features(flappy_bird PRIVATE cxx_std_20)

target_include_directories(flappy_bird PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(flappy_bird PRIVATE
        caffeine_gl
)

