#include "Game.h"
#include "Asteroid.h"
#include "Player.h"

std::vector<Entity*> entities{};
std::list<std::vector<Entity*>::const_iterator> toRemoveList;
std::list<Entity*> toAddList{};
float Game::asteroidSpawnTime{};

void Game::begin() {
    entities.push_back(new Player());
    // entities.push_back(new Asteroid(Asteroid::getRandomDirection()));
    float asteroidSpawnTime = ASTEROID_SPAWN_TIME;
}

void Game::update(sf::RenderWindow& window, float deltaTime) {
    toAddList.clear();
    toRemoveList.clear();
    window.clear();

    asteroidSpawnTime -= deltaTime;

    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->update(deltaTime);
        entities[i]->render(window);
    }

    for (const auto &it : toRemoveList)
    {
        delete *it;
        entities.erase(it);
    }

    for (auto &ptr : toAddList)
    {
        entities.push_back(ptr);
    }

    if (asteroidSpawnTime <= 0.0f)
    {
        entities.push_back(new Asteroid());
        asteroidSpawnTime = ASTEROID_SPAWN_TIME;
    }
}
