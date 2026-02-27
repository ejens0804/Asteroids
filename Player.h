#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

constexpr float PLAYER_W        = 50.0f;
constexpr float PLAYER_H        = 40.0f;
constexpr float TURN_SPEED      = 200.0f;
constexpr float THRUST_FORCE    = 420.0f;  // acceleration
// constexpr float PLAYER_SPEED    = 400.0f;
constexpr float DRAG            = 0.985f;   // velocity multiplier per frame
constexpr float MAX_SPEED       = 700.0f;
constexpr float SHOOT_DELAY     = 0.22f;
constexpr float INVINCIBLE_TIME = 2.5f;    // seconds after taking a hit

class Player : public Entity {
public:
    Player();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    std::vector<sf::Vector2f> getLocalVertices() const override;

    // Called by Game when the player takes a hit
    void onHit();
    bool isInvincible() const { return invincibleTimer > 0.f; }

private:
    sf::VertexArray array;
    sf::VertexArray thrustArray;  // engine flame
    sf::Vector2f    velocity;
    float           shootTimer;
    float           invincibleTimer;
    bool            thrusting;

    void wrapPosition();
};
