#include "Asteroid.h"
#include "Entity.h"
#include "Constants.h"
#include <random>


class Asteroid : public Entity
{
public:
    Asteroid(sf::Vector2f direction = Asteroid::getRandomDirection(), 
    sf::Vector2f position = Asteroid::getRandomPosition())
        : Entity(position, 0), direction(direction), array(sf::LinesStrip, 12)
    {
        array[0].position = sf::Vector2f(-40, 40);
        array[1].position = sf::Vector2f(-50, 10);
        array[2].position = sf::Vector2f(-10, -20);
        array[3].position = sf::Vector2f(-20, -40);
        array[4].position = sf::Vector2f(10, -40);
        array[5].position = sf::Vector2f(40, -20);
        array[6].position = sf::Vector2f(40, -10);
        array[7].position = sf::Vector2f(10, 0);
        array[8].position = sf::Vector2f(40, 20);
        array[9].position = sf::Vector2f(20, 40);
        array[10].position = sf::Vector2f(0, 30);
        array[11].position = array[0].position;

        for (size_t i = 0; i < array.getVertexCount(); i++)
        {
            array[i].color = sf::Color::White;
        }
    }

    void update(float deltaTime) override
    {
        position += ASTEROID_SPEED * direction * deltaTime;
        angle += ASTEROID_SPIN * deltaTime;

        if (position.x < ASTEROID_W / 2.0f)
        {
            direction.x = std::abs(direction.x);
        }
        else if (position.x > SCREEN_WIDTH - ASTEROID_W / 2.0f)
        {
            direction.x = -std::abs(direction.x);
        }

        if (position.y < ASTEROID_H / 2.0f)
        {
            direction.y = std::abs(direction.y);
        }
        else if (position.y > SCREEN_HEIGHT - ASTEROID_H / 2.0f)
        {
            direction.y = -std::abs(direction.y);
        }
    }

    void render(sf::RenderWindow &window) override
    {
        // sf::Transform transform;
        window.draw(array, sf::Transform().translate(position).rotate(angle));
    } // & means a reference to the item that it is typed next to

    static sf::Vector2f getRandomDirection()
    {
        static std::mt19937 gen(std::random_device{}());
        static std::uniform_real_distribution<float> dist(0.0f, 2.0f * PI);

        float angle = dist(gen);
        return sf::Vector2f(cos(angle), sin(angle));
    }

    static sf::Vector2f getRandomPosition()
    {
        static std::mt19937 gen(std::random_device{}());

        static std::uniform_real_distribution<float> xAxis(ASTEROID_W / 2.0f,
            SCREEN_WIDTH - ASTEROID_W / 2.0f);

        static std::uniform_real_distribution<float> yAxis(ASTEROID_H / 2.0f,
            SCREEN_HEIGHT - ASTEROID_H / 2.0f);

        return sf::Vector2f(xAxis(gen), yAxis(gen));
    }

private:
    sf::VertexArray array;
    sf::Vector2f direction;
};