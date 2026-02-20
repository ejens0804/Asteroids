#pragma once 
#include "Entity.h"

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