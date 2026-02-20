#pragma once 
#include <vector>
#include <list>
#include "Entity.h"



extern std::vector<Entity *> entities{};
extern std::list<std::vector<Entity *>::const_iterator> toRemoveList;
extern std::list<Entity *> toAddList{};