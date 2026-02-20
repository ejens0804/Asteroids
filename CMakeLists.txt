cmake_minimum_required(VERSION 3.16)
project(Asteroids)

set(CMAKE_PREFIX_PATH "C:/msys64/mingw64")

find_package(SFML 2.6 COMPONENTS graphics window audio system REQUIRED)

add_executable(test 
    main.cpp
    Entity.cpp
    Bullet.cpp
    Player.cpp
    Asteroid.cpp
    Globals.cpp
    )

target_link_libraries(test sfml-graphics sfml-window sfml-audio sfml-system)

set_target_properties(test PROPERTIES WIN32_EXECUTABLE FALSE)