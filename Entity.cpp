#include "Entity.h"

Entity::Entity(sf::Vector2f position, float angle)
        : position(position), angle(angle) {}

    void update(float deltaTime);
    void render(sf::RenderWindow &window);

    sf::Vector2f position;
    float angle; // the angle the player is facing