#include "Entity.h"


class Entity
{
public:
    Entity(sf::Vector2f position, float angle)
        : position(position), angle(angle) {}

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

    sf::Vector2f position;
    float angle; // the angle the player is facing
};