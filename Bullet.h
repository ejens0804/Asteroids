#pragma once 
#include "Entity.h"

constexpr float BULLET_SPEED = 400.0f;
constexpr float BULLET_LIFE = 3.0f;

class Bullet : public Entity
{
public:
    Bullet(sf::Vector2f position, sf::Vector2f direction);

    void update(float deltaTime) override;

    void render(sf::RenderWindow &window) override;

private:
    sf::Vector2f direction;
    sf::CircleShape shape;
    float lifetime;
};