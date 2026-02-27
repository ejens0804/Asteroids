#include "Bullet.h"
#include "Global.h"

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f dir)
    : Entity(pos, 0.f),
      shape(BULLET_RADIUS),
      direction(dir),
      lifetime(BULLET_LIFE)
{
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(BULLET_RADIUS, BULLET_RADIUS);
}

void Bullet::update(float deltaTime) {
    lifetime -= deltaTime;
    if (lifetime <= 0.f) {
        alive = false;
        return;
    }
    position += direction * BULLET_SPEED * deltaTime;
    wrapPosition();
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(shape, sf::Transform().translate(position));
}

void Bullet::wrapPosition() {
    if (position.x < 0.f)            position.x += SCREEN_WIDTH;
    if (position.x > SCREEN_WIDTH)   position.x -= SCREEN_WIDTH;
    if (position.y < 0.f)            position.y += SCREEN_HEIGHT;
    if (position.y > SCREEN_HEIGHT)  position.y -= SCREEN_HEIGHT;
}
