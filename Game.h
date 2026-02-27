#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Global.h"

class Game {
public:
    static bool init();
    static void begin();
    static void update(sf::RenderWindow& window, float deltaTime);
    static void handleEvent(const sf::Event& e);

private:
    static sf::Font font;
    static int currentLevel;

    static void loadHighScore();
    static void saveHighScore();

    static void updateMainMenu (sf::RenderWindow& window);
    static void updatePlaying  (sf::RenderWindow& window, float deltaTime);
    static void updateGameOver (sf::RenderWindow& window);

    static void checkCollisions();

    static void drawHUD        (sf::RenderWindow& window);
    static void drawCentredText(sf::RenderWindow& window, const std::string& str,
                                unsigned size, float yFraction,
                                sf::Color colour = sf::Color::White);
    static void drawLives      (sf::RenderWindow& window);

    static void pruneDeadEntities();
    static void flushAddList();
    static void clearEntities();

    static bool anyAsteroidsAlive();
};