#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity(sf::Vector2f position, float angle);
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    sf::Vector2f position;
    float angle;
};