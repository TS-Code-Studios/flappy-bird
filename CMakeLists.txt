cmake_minimum_required(VERSION 3.31)
project(flappy_bird)

add_subdirectory(caffeine-gl)

set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)

add_executable(flappy_bird
    src/main.cpp
        src/game/FlappyBirdGame.cpp
        include/game/FlappyBirdGame.hpp
        include/game/PipePair.hpp
        include/components/PlayerMovementComponent.hpp
        include/systems/PlayerMovementSystem.hpp
)

target_compile_features(flappy_bird PRIVATE cxx_std_20)

target_include_directories(flappy_bird PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(flappy_bird PRIVATE
        caffeine_gl
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/resources
)
