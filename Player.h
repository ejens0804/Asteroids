#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>


class Player : public Entity
{
public:
    Player();

    void update(float deltaTime) override;
    
    void render(sf::RenderWindow &window) override;

private:
    sf::VertexArray array;
    float shootTimer;
};