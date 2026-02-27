#include "Player.h"
#include "Entity.h"
#include "Constants.h"
#include "Bullet.h"
#include "Global.h"
#include "SoundManager.h"
#include <cmath>
#include <algorithm>

Player::Player()
    : Entity(sf::Vector2f(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f), 0.f),
      array(sf::LinesStrip, 5),
      thrustArray(sf::LinesStrip, 4),
      velocity(0.f, 0.f),
      shootTimer(0.f),
      invincibleTimer(0.f),
      thrusting(false)
{
    // Ship shape (nose right)
    array[0].position = sf::Vector2f( 20,   0);
    array[1].position = sf::Vector2f(-30, -20);
    array[2].position = sf::Vector2f(-15,   0);
    array[3].position = sf::Vector2f(-30,  20);
    array[4].position = array[0].position;
    for (unsigned i = 0; i < array.getVertexCount(); ++i)
        array[i].color = sf::Color::White;

    // Thrust flame (drawn behind ship)
    thrustArray[0].position = sf::Vector2f(-15, -10);
    thrustArray[1].position = sf::Vector2f(-40,   0);
    thrustArray[2].position = sf::Vector2f(-15,  10);
    thrustArray[3].position = thrustArray[0].position;
    for (unsigned i = 0; i < thrustArray.getVertexCount(); ++i)
        thrustArray[i].color = sf::Color(255, 160, 40);
}

// ─── Update ──────────────────────────────────────────────────────────────────
void Player::update(float deltaTime) {
    shootTimer      -= deltaTime;
    invincibleTimer  = std::max(0.f, invincibleTimer - deltaTime);
    thrusting        = false;

    // Rotation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        angle -= TURN_SPEED * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        angle += TURN_SPEED * deltaTime;

    // Thrust (classic Asteroids: no reverse, just momentum)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        thrusting = true;
        float rad = angle * (PI / 180.f);
        velocity.x += std::cos(rad) * THRUST_FORCE * deltaTime;
        velocity.y += std::sin(rad) * THRUST_FORCE * deltaTime;

        // Play thrust sound every so often
        static float thrustSoundTimer = 0.f;
        thrustSoundTimer -= deltaTime;
        if (thrustSoundTimer <= 0.f) {
            SoundManager::instance().play(SoundManager::Sound::THRUST);
            thrustSoundTimer = 0.12f;
        }
    }

    // Drag / speed cap
    velocity *= DRAG;
    float spd = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (spd > MAX_SPEED) velocity *= MAX_SPEED / spd;

    position += velocity * deltaTime;
    wrapPosition();

    // Shoot
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer <= 0.f) {
        shootTimer = SHOOT_DELAY;
        float rad  = angle * (PI / 180.f);
        toAddList.push_back(
            new Bullet(position, sf::Vector2f(std::cos(rad), std::sin(rad))));
        SoundManager::instance().play(SoundManager::Sound::SHOOT);
    }
}

// ─── Render ──────────────────────────────────────────────────────────────────
void Player::render(sf::RenderWindow& window) {
    // Blink during invincibility
    if (invincibleTimer > 0.f) {
        int frame = static_cast<int>(invincibleTimer * 10.f);
        if (frame % 2 == 0) return;
    }

    sf::Transform t = sf::Transform().translate(position).rotate(angle);
    if (thrusting)
        window.draw(thrustArray, t);
    window.draw(array, t);
}

// ─── Collision polygon (3 vertices of ship triangle) ─────────────────────────
std::vector<sf::Vector2f> Player::getLocalVertices() const {
    return {
        {  20,   0},   // nose
        { -30, -20},   // back-left
        { -30,  20}    // back-right
    };
}

// ─── Screen wrap ─────────────────────────────────────────────────────────────
void Player::wrapPosition() {
    if (position.x < 0.f)            position.x += SCREEN_WIDTH;
    if (position.x > SCREEN_WIDTH)   position.x -= SCREEN_WIDTH;
    if (position.y < 0.f)            position.y += SCREEN_HEIGHT;
    if (position.y > SCREEN_HEIGHT)  position.y -= SCREEN_HEIGHT;
}

// ─── Hit handler ─────────────────────────────────────────────────────────────
void Player::onHit() {
    invincibleTimer = INVINCIBLE_TIME;
    velocity        = { 0.f, 0.f };
    position        = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
}
