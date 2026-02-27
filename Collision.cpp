#include "Collision.h"
#include <cmath>
#include <algorithm>
#include <limits>

namespace Collision {

float dot(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.x + a.y * b.y;
}

Projection project(const std::vector<sf::Vector2f>& poly, sf::Vector2f axis) {
    float mn =  std::numeric_limits<float>::infinity();
    float mx = -std::numeric_limits<float>::infinity();
    for (const auto& v : poly) {
        float d = dot(v, axis);
        mn = std::min(mn, d);
        mx = std::max(mx, d);
    }
    return { mn, mx };
}

bool overlaps(Projection a, Projection b) {
    return a.min <= b.max && b.min <= a.max;
}

// ─── SAT helper: test axes from one polygon against both polygons ───────────
static bool testAxes(const std::vector<sf::Vector2f>& source,
                     const std::vector<sf::Vector2f>& polyA,
                     const std::vector<sf::Vector2f>& polyB)
{
    for (size_t i = 0; i < source.size(); ++i) {
        sf::Vector2f edge = source[(i + 1) % source.size()] - source[i];
        // Perpendicular (outward normal)
        sf::Vector2f axis = { -edge.y, edge.x };
        // Normalize
        float len = std::sqrt(axis.x * axis.x + axis.y * axis.y);
        if (len < 1e-6f) continue;
        axis /= len;

        Projection pA = project(polyA, axis);
        Projection pB = project(polyB, axis);
        if (!overlaps(pA, pB))
            return false; // Separating axis found = no collision
    }
    return true;
}

bool polygonsIntersect(const std::vector<sf::Vector2f>& polyA,
                       const std::vector<sf::Vector2f>& polyB)
{
    if (polyA.empty() || polyB.empty()) return false;
    // Test axes from both polygons
    return testAxes(polyA, polyA, polyB) &&
           testAxes(polyB, polyA, polyB);
}

bool pointInPolygon(sf::Vector2f point,
                    const std::vector<sf::Vector2f>& poly)
{
    if (poly.empty()) return false;
    // Ray-casting algorithm (found online)
    int crosses = 0;
    size_t n = poly.size();
    for (size_t i = 0; i < n; ++i) {
        sf::Vector2f a = poly[i];
        sf::Vector2f b = poly[(i + 1) % n];
        if (((a.y <= point.y && point.y < b.y) ||
             (b.y <= point.y && point.y < a.y)) &&
            (point.x < (b.x - a.x) * (point.y - a.y) / (b.y - a.y) + a.x))
            ++crosses;
    }
    return (crosses % 2) == 1;
}

bool circleIntersectsPolygon(sf::Vector2f centre, float radius,
                             const std::vector<sf::Vector2f>& poly)
{
    if (poly.empty()) return false;
    size_t n = poly.size();

    // 1) Is center inside polygon
    if (pointInPolygon(centre, poly)) return true;

    // 2) Is any edge closer than radius
    for (size_t i = 0; i < n; ++i) {
        sf::Vector2f a = poly[i];
        sf::Vector2f b = poly[(i + 1) % n];
        sf::Vector2f ab = b - a;
        sf::Vector2f ac = centre - a;
        float t = dot(ac, ab) / dot(ab, ab);
        t = std::max(0.0f, std::min(1.0f, t));
        sf::Vector2f closest = a + t * ab;
        sf::Vector2f diff = centre - closest;
        float distSq = diff.x * diff.x + diff.y * diff.y;
        if (distSq <= radius * radius) return true;
    }
    return false;
}

}
