#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <list>

// command to run in terminal to run program:
// cd /c/dev/cplusplus/Asteroids/build && ./test.exe
constexpr float PI = 3.14159265f;

constexpr float TURN_SPEED = 200.0f;
constexpr float PLAYER_SPEED = 200.0f;
constexpr float SHOOT_DELAY = 0.2f;
constexpr float BULLET_SPEED = 400.0f;
constexpr float Bullet_LIFE = 3.0f;

class Entity 
{
public:
    Entity(sf::Vector2f position, float angle):
        position(position), angle(angle) {}

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    sf::Vector2f position;
    float angle; // the angle the player is facing
};




std::vector<Entity*> entities{};
std::list<std::vector<Entity*>::const_iterator> toRemoveList;



class Bullet : public Entity{
public:
    Bullet(sf::Vector2f position, sf::Vector2f direction):
        shape(1.0f), direction(direction), Entity(position, 0.0f), lifetime(Bullet_LIFE) {}

    void update(float deltaTime) override {
        lifetime -= deltaTime;
        position += direction * BULLET_SPEED * deltaTime;
        if (lifetime <= 0.0f){
            toRemoveList.push_back(std::find(entities.begin(), entities.end(), this));
        }
    }

    void render(sf::RenderWindow& window) override {
        window.draw(shape, sf::Transform().translate(position));
    }

private:
    sf::Vector2f direction;
    sf::CircleShape shape;
    float lifetime;
};





class Player : public Entity
{
public:
    Player(): 
        Entity(sf::Vector2f(500, 500),0), array(sf::Quads, 4), shootTimer()
    {
        array[0].position = sf::Vector2f(20, 0);
        array[1].position = sf::Vector2f(-30, -20);
        array[2].position = sf::Vector2f(-15, 0);
        array[3].position = sf::Vector2f(-30, 20);

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
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            float radians = angle * (PI / 180.0f);

            position.x -= cos(radians) * PLAYER_SPEED * deltaTime;
            position.y -= sin(radians) * PLAYER_SPEED * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer <= 0.0f){
            shootTimer = SHOOT_DELAY;
            float radians = angle * (PI / 180.0f);

            entities.push_back(
                new Bullet(position, sf::Vector2f(cos(radians), sin(radians))));
        }
    }

    void render(sf::RenderWindow &window)
    {
        sf::Transform transform;
        window.draw(array, sf::Transform().translate(position).rotate(angle));
    } // & means a reference to the item that it is typed next to

    

private:
    sf::VertexArray array;
    float shootTimer;
};





int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Asteroids Game",
                            sf::Style::Close | sf::Style::Titlebar);
    sf::Clock clock;

    
    entities.push_back(new Player());

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        sf::Event e{};
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Q) {
                    printf("%d\n", entities.size());
                }
            }
        }

        toRemoveList.clear();
        window.clear();
        for (size_t i = 0; i < entities.size();i++){
            entities[i]->update(deltaTime);
            entities[i]->render(window);
        }

        for (const auto& it : toRemoveList){
            delete* it;
            entities.erase(it);
        }

        window.display(); // draw the game
    }
}