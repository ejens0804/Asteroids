#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Entity {
public:
    Entity(sf::Vector2f position, float angle);
    virtual ~Entity() = default;

    virtual void update(float deltaTime)       = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    // Return convex polygon vertices in LOCAL space (no transform applied).
    // Used by the SAT collision system.
    virtual std::vector<sf::Vector2f> getLocalVertices() const { return {}; }

    // Convenience: apply position + angle transform to local vertices.
    std::vector<sf::Vector2f> getWorldVertices() const;

    sf::Vector2f position;
    float        angle;
    bool         alive = true;   // set false to queue for deletion
};
