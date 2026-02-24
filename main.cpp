#include <iostream>
#include <SFML/Graphics.hpp>

// Link files to program
#include "Entity.h"
#include "Player.h"
#include "Asteroid.h"
#include "Game.h"

// command to run in terminal to run program:
// cd build && ./test.exe

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Asteroids Game",
                            sf::Style::Close | sf::Style::Titlebar);
    sf::Clock clock;

    Game::begin();

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        sf::Event e{};
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            // else if (e.type == sf::Event::KeyPressed)
            // {
            //     if (e.key.code == sf::Keyboard::Q)
            //     {
            //         printf("%d\n", entities.size());
            //     }
            // }
        }

        Game::update(window, deltaTime);

        window.display(); // draw the game
    }
}