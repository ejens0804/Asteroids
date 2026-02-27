#include "Global.h"
#include "Entity.h"

std::vector<Entity*> entities;
std::list<Entity*>   toAddList;
int        score     = 0;
int        lives     = 3;
int        highScore = 0;
GameState  gameState = GameState::MAIN_MENU;
