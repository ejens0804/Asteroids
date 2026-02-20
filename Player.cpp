#include "Player.h"
#include "Entity.h"
#include "Constants.h"
#include <cmath>
#include "Bullet.h"
#include "Globals.h"


class Player : public Entity
{
public:
    Player() 
        : Entity(sf::Vector2f(500, 500), 0), array(sf::LinesStrip, 5), shootTimer()
    {
        array[0].position = sf::Vector2f(20, 0);
        array[1].position = sf::Vector2f(-30, -20);
        array[2].position = sf::Vector2f(-15, 0);
        array[3].position = sf::Vector2f(-30, 20);
        array[4].position = array[0].position;

        for (size_t i = 0; i < array.getVertexCount(); i++)
        {
            array[i].color = sf::Color::White;
        }
    }

    void update(float deltaTime) override
    {
        shootTimer -= deltaTime;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            angle -= TURN_SPEED * deltaTime;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            angle += TURN_SPEED * deltaTime;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            float radians = angle * (PI / 180.0f);

            position.x += cos(radians) * PLAYER_SPEED * deltaTime;
            position.y += sin(radians) * PLAYER_SPEED * deltaTime;

            position.x = std::min(std::max(position.x, PLAYER_W / 2.0f),
                                  SCREEN_WIDTH - PLAYER_W / 2.0f);
            position.y = std::min(std::max(position.y, PLAYER_H / 2.0f),
                                  SCREEN_HEIGHT - PLAYER_H / 2.0f);
        }

        // TURN THIS SECTION OFF LATER TO MIMIC THE ORIGINAL ASTEROIDS
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            float radians = angle * (PI / 180.0f);

            position.x -= cos(radians) * PLAYER_SPEED * deltaTime;
            position.y -= sin(radians) * PLAYER_SPEED * deltaTime;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer <= 0.0f)
        {
            shootTimer = SHOOT_DELAY;
            float radians = angle * (PI / 180.0f);

            toAddList.push_back(
                new Bullet(position, sf::Vector2f(cos(radians), sin(radians))));
        }
    }

    void render(sf::RenderWindow &window) override
    {
        // sf::Transform transform; // LINE OF CODE THAT MAY NEED TO BE DELETED>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        window.draw(array, sf::Transform().translate(position).rotate(angle));
    } // & means a reference to the item that it is typed next to

private:
    sf::VertexArray array;
    float shootTimer;
};