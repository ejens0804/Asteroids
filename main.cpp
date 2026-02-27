#include <SFML/Graphics.hpp>
#include <iostream>

#include "Global.h"
#include "Game.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(static_cast<unsigned>(SCREEN_WIDTH),
                      static_cast<unsigned>(SCREEN_HEIGHT)),
        "Asteroids",
        sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(120);

    if (!Game::init())
        std::cerr << "[Warning] Could not load a font – text will be invisible.\n"
                  << "          Place a .ttf file at  assets/font.ttf  in your build dir.\n";

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        sf::Event e{};
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
            else
                Game::handleEvent(e);
        }

        Game::update(window, deltaTime);
    }

    return 0;
}
