cmake_minimum_required(VERSION 3.16)
project(Asteroids)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/msys64/mingw64")

find_package(SFML 2.6 COMPONENTS graphics window audio system REQUIRED)

set(SOURCES
    main.cpp
    Entity.cpp
    Globals.cpp
    Bullet.cpp
    Player.cpp
    Asteroid.cpp
    Collision.cpp
    SoundManager.cpp
    Game.cpp
)

add_executable(Asteroids ${SOURCES})

target_link_libraries(Asteroids
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-system
)

# Keep as a console app so startup errors are visible
set_target_properties(Asteroids PROPERTIES WIN32_EXECUTABLE FALSE)

# ─── Copy assets to the build directory ──────────────────────────────────────
# Drop any .ttf file into the project root "assets/" folder and it will be
# copied alongside the executable automatically.
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET Asteroids POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:Asteroids>/assets"
        COMMENT "Copying assets to build directory"
    )
endif()
