#pragma once
#include "Entity.h"

constexpr float BULLET_SPEED  = 600.0f;
constexpr float BULLET_LIFE   = 1.4f;
constexpr float BULLET_RADIUS = 3.0f;

class Bullet : public Entity {
public:
    Bullet(sf::Vector2f position, sf::Vector2f direction);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    // Returns radius for circle-vs-polygon collision
    float getRadius() const { return BULLET_RADIUS; }

private:
    sf::CircleShape shape;
    sf::Vector2f    direction;
    float           lifetime;

    void wrapPosition();
};
