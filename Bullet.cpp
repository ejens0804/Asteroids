#include "Bullet.h"
#include "Globals.h"
#include "Constants.h"
#include <algorithm>


Bullet::Bullet(sf::Vector2f position, sf::Vector2f direction) : shape(1.0f), direction(direction), Entity(position, 0.0f), lifetime(BULLET_LIFE) {}

    void Bullet::update(float deltaTime)
    {
        lifetime -= deltaTime;
        position += direction * BULLET_SPEED * deltaTime;
        if (lifetime <= 0.0f)
        {
            toRemoveList.push_back(std::find(entities.begin(), entities.end(), this));
        }
    }

    void Bullet::render(sf::RenderWindow &window) 
    {
        window.draw(shape, sf::Transform().translate(position));
    }