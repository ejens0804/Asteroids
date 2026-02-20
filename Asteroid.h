#pragma once


class Asteroid : public Entity
{
public:
    Asteroid(sf::Vector2f direction = Asteroid::getRandomDirection(), 
    sf::Vector2f position = Asteroid::getRandomPosition());

    void update(float deltaTime) override;

    void render(sf::RenderWindow &window) override;
    
    static sf::Vector2f getRandomDirection();

    static sf::Vector2f getRandomPosition();

private:
    sf::VertexArray array;
    sf::Vector2f direction;
};