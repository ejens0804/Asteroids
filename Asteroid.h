#pragma once

#include "Entity.h"
#include "Global.h"

constexpr float ASTEROID_W = 90.0f;
constexpr float ASTEROID_H = 80.0f;
constexpr float ASTEROID_SPIN = 25.0f;
constexpr float ASTEROID_SPEED = 80.0f; // Default: 80.0f
constexpr float ASTEROID_SPAWN_TIME = 3.0f;

class Asteroid : public Entity
{
public:
    Asteroid(sf::Vector2f direction = Asteroid::getRandomDirection(), 
    sf::Vector2f position = Asteroid::getRandomPosition());

    void update(float deltaTime) override;

    void render(sf::RenderWindow &window) override;
    
    static sf::Vector2f getRandomDirection();

    static sf::Vector2f getRandomPosition();

private:
    sf::VertexArray array;
    sf::Vector2f direction;
};