#include <iostream>
#include <SFML/Graphics.hpp>

// Link files to program
#include "Constants.h"
#include "Entity.h"
#include "Globals.h"
#include "Player.h"
#include "Asteroid.h"

// command to run in terminal to run program:
// cd build && ./test.exe

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Asteroids Game",
                            sf::Style::Close | sf::Style::Titlebar);
    sf::Clock clock;

    entities.push_back(new Player());
    entities.push_back(new Asteroid(Asteroid::getRandomDirection()));

    float asteroidSpawnTime = ASTEROID_SPAWN_TIME;

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
            else if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::Q)
                {
                    printf("%d\n", entities.size());
                }
            }
        }

        toAddList.clear();
        toRemoveList.clear();
        window.clear();

        asteroidSpawnTime -= deltaTime;

        for (size_t i = 0; i < entities.size(); i++)
        {
            entities[i]->update(deltaTime);
            entities[i]->render(window);
        }

        for (const auto &it : toRemoveList)
        {
            delete *it;
            entities.erase(it);
        }

        for (auto &ptr : toAddList)
        {
            entities.push_back(ptr);
        }

        if (asteroidSpawnTime <= 0.0f)
        {
            entities.push_back(new Asteroid());
            asteroidSpawnTime = ASTEROID_SPAWN_TIME;
        }

        window.display(); // draw the game
    }
}