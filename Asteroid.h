#pragma once
#include "Entity.h"
#include "Global.h"

constexpr float ASTEROID_SPIN_LARGE  = 15.0f;
constexpr float ASTEROID_SPIN_MEDIUM = 30.0f;
constexpr float ASTEROID_SPIN_SMALL  = 55.0f;

constexpr float ASTEROID_SPEED_LARGE  =  60.0f;
constexpr float ASTEROID_SPEED_MEDIUM = 110.0f;
constexpr float ASTEROID_SPEED_SMALL  = 180.0f;

constexpr float ASTEROID_SPAWN_TIME = 4.0f;   // seconds between auto-spawns

enum class AsteroidSize { LARGE, MEDIUM, SMALL };

class Asteroid : public Entity {
public:
    // Main constructor – all arguments have defaults so it can be used
    // directly without arguments for random spawn.
    explicit Asteroid(AsteroidSize size      = AsteroidSize::LARGE,
                      sf::Vector2f direction = getRandomDirection(),
                      sf::Vector2f position  = getRandomEdgePosition());

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    std::vector<sf::Vector2f> getLocalVertices() const override;

    AsteroidSize getSize() const { return size; }

    static sf::Vector2f getRandomDirection();
    static sf::Vector2f getRandomEdgePosition();  // spawn on screen edge

private:
    AsteroidSize   size;
    sf::Vector2f   direction;
    sf::VertexArray array;
    float           speed;
    float           spinRate;

    // Local-space vertices mirroring the draw array (without closing vertex)
    std::vector<sf::Vector2f> localVerts;

    void buildShape();
    void wrapPosition();
};
