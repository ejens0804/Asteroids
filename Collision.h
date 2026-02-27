#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Separating Axis Theorem (SAT) collision utilities.
// All functions work on convex polygons represented as ordered vertex lists.

namespace Collision {

    // Project polygon onto axis and return [min, max] interval.
    struct Projection { float min, max; };
    Projection project(const std::vector<sf::Vector2f>& poly, sf::Vector2f axis);

    // Do two 1-D intervals overlap?
    bool overlaps(Projection a, Projection b);

    // Dot product helper.
    float dot(sf::Vector2f a, sf::Vector2f b);

    // Full SAT test between two convex polygons.
    // Returns true when they intersect.
    bool polygonsIntersect(const std::vector<sf::Vector2f>& polyA,
                           const std::vector<sf::Vector2f>& polyB);

    // Point-vs-convex-polygon test (faster than full SAT for tiny objects).
    bool pointInPolygon(sf::Vector2f point,
                        const std::vector<sf::Vector2f>& poly);

    // Circle-vs-polygon (used for circular bullet radius).
    bool circleIntersectsPolygon(sf::Vector2f centre, float radius,
                                 const std::vector<sf::Vector2f>& poly);
}
