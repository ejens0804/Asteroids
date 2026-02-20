#include "Globals.h"

std::vector<Entity*> entities{};
std::list<std::vector<Entity*>::const_iterator> toRemoveList;
std::list<Entity*> toAddList{};