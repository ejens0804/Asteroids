# Asteroids

A classic Asteroids arcade game clone built in C++ using the SFML library. The game features procedurally generated sound effects, SAT-based polygon collision detection, screen wrapping, multiple asteroid sizes that split on destruction, progressive difficulty levels, and a persistent high score system.

## Instructions for Build and Use

Steps to build and/or run the software:

1. Install SFML 2.6 and CMake 3.16 or later (on Windows, MSYS2/MinGW64 is recommended)
2. Clone or download the repository, then from the project root run: `cmake -B build` followed by `cmake --build build`
3. Run the resulting `Asteroids` executable from the `build/` directory. Optionally place a `font.ttf` file in `build/assets/` for visible text; the game will search common system font paths as a fallback.

Instructions for using the software:

1. Launch the game and press **Enter** on the main menu to start playing
2. Use **W / Up Arrow** to thrust, **A / Left Arrow** and **D / Right Arrow** to rotate, and **Space** to fire bullets at asteroids
3. Destroy all asteroids to advance to the next level; avoid being hit — you have 3 lives. On Game Over, press **Enter** to play again or **Escape** to return to the main menu.

## Development Environment

To recreate the development environment, you need the following software and/or libraries with the specified versions:

* C++20 compatible compiler (e.g. GCC via MSYS2/MinGW64 on Windows)
* SFML 2.6 (Graphics, Window, Audio, System components)
* CMake 3.16 or later

## Useful Websites to Learn More

I found these websites useful in developing this software:

* [SFML Official Documentation](https://www.sfml-dev.org/documentation/2.6.0/)
* [Separating Axis Theorem (SAT) Explained](https://dyn4j.org/2010/01/sat/)
* [MSYS2 / MinGW64 Setup Guide](https://www.msys2.org/)

## Future Work

The following items I plan to fix, improve, and/or add to this project in the future:

* [ ] Add particle explosion effects when asteroids are destroyed
* [ ] Implement a UFO enemy that shoots at the player periodically
* [ ] Add a local leaderboard that stores the top 10 scores with player initials
* [ ] Customize the window size and make it resizeable