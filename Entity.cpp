#include "Entity.h"

Entity::Entity(sf::Vector2f position, float angle)
    : position(position), angle(angle) {}

std::vector<sf::Vector2f> Entity::getWorldVertices() const {
    sf::Transform t;
    t.translate(position).rotate(angle);
    auto local = getLocalVertices();
    std::vector<sf::Vector2f> world;
    world.reserve(local.size());
    for (const auto& v : local)
        world.push_back(t.transformPoint(v));
    return world;
}
