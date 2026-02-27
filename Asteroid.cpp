#include "Asteroid.h"
#include <random>
#include <cmath>

// ─── Random helpers ──────────────────────────────────────────────────────────
sf::Vector2f Asteroid::getRandomDirection() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.0f, 2.0f * PI);
    float a = dist(gen);
    return { std::cos(a), std::sin(a) };
}

sf::Vector2f Asteroid::getRandomEdgePosition() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> xDist(0.f, SCREEN_WIDTH);
    static std::uniform_real_distribution<float> yDist(0.f, SCREEN_HEIGHT);
    static std::uniform_int_distribution<int>    edgeDist(0, 3);

    switch (edgeDist(gen)) {
        case 0: return { xDist(gen),  -60.f };                // top
        case 1: return { xDist(gen),   SCREEN_HEIGHT + 60.f };// bottom
        case 2: return { -60.f,        yDist(gen) };           // left
        default:return { SCREEN_WIDTH + 60.f, yDist(gen) };   // right
    }
}

// ─── Asteroid shape data per size ─────────────────────────────────────────────────────
//  All shapes are roughly centred at origin.
//  Using LineStrip with closing vertex for rendering,
//  and a separate localVerts list (no closing vertex) for collision.

static const std::vector<sf::Vector2f> SHAPE_LARGE = {
    {-40,  40}, {-50,  10}, {-10, -20}, {-20, -40},
    { 10, -40}, { 40, -20}, { 40, -10}, { 10,   0},
    { 40,  20}, { 20,  40}, {  0,  30}
};

static const std::vector<sf::Vector2f> SHAPE_MEDIUM = {
    {-20,  20}, {-25,   5}, { -5, -10}, {-10, -20},
    {  5, -20}, { 20, -10}, { 20,  -5}, {  5,   0},
    { 20,  10}, { 10,  20}, {  0,  15}
};

static const std::vector<sf::Vector2f> SHAPE_SMALL = {
    {-10,  10}, {-12,   3}, { -3,  -5}, { -5, -10},
    {  3, -10}, { 10,  -5}, { 10,  -2}, {  3,   0},
    { 10,   5}, {  5,  10}, {  0,   7}
};

// ─── Constructor ─────────────────────────────────────────────────────────────
Asteroid::Asteroid(AsteroidSize size, sf::Vector2f direction, sf::Vector2f position)
    : Entity(position, 0.f), size(size), direction(direction)
{
    switch (size) {
        case AsteroidSize::LARGE:
            speed    = ASTEROID_SPEED_LARGE;
            spinRate = ASTEROID_SPIN_LARGE;
            localVerts = SHAPE_LARGE;
            break;
        case AsteroidSize::MEDIUM:
            speed    = ASTEROID_SPEED_MEDIUM;
            spinRate = ASTEROID_SPIN_MEDIUM;
            localVerts = SHAPE_MEDIUM;
            break;
        case AsteroidSize::SMALL:
            speed    = ASTEROID_SPEED_SMALL;
            spinRate = ASTEROID_SPIN_SMALL;
            localVerts = SHAPE_SMALL;
            break;
    }
    buildShape();
}

void Asteroid::buildShape() {
    array = sf::VertexArray(sf::LinesStrip,
                            static_cast<unsigned>(localVerts.size()) + 1);
    for (size_t i = 0; i < localVerts.size(); ++i) {
        array[i].position = localVerts[i];
        array[i].color    = sf::Color::White;
    }
    // Closing vertex
    array[localVerts.size()].position = localVerts[0];
    array[localVerts.size()].color    = sf::Color::White;
}

// ─── Update ──────────────────────────────────────────────────────────────────
void Asteroid::update(float deltaTime) {
    position += direction * speed * deltaTime;
    angle    += spinRate * deltaTime;
    wrapPosition();
}

void Asteroid::wrapPosition() {
    // Classic Asteroids screen-wrap
    const float MARGIN = 80.f;
    if (position.x < -MARGIN)              position.x += SCREEN_WIDTH  + MARGIN * 2;
    if (position.x > SCREEN_WIDTH + MARGIN) position.x -= SCREEN_WIDTH  + MARGIN * 2;
    if (position.y < -MARGIN)              position.y += SCREEN_HEIGHT + MARGIN * 2;
    if (position.y > SCREEN_HEIGHT+ MARGIN) position.y -= SCREEN_HEIGHT + MARGIN * 2;
}

// ─── Render ──────────────────────────────────────────────────────────────────
void Asteroid::render(sf::RenderWindow& window) {
    window.draw(array, sf::Transform().translate(position).rotate(angle));
}

// ─── Collision vertices ───────────────────────────────────────────────────────
std::vector<sf::Vector2f> Asteroid::getLocalVertices() const {
    return localVerts;
}
