#pragma once
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>

// Forward declarations
class Entity;

// Screen dimensions
constexpr float SCREEN_WIDTH  = 1920.0f;
constexpr float SCREEN_HEIGHT = 1080.0f;
constexpr float PI = 3.14159265f;

// Game states
enum class GameState {
    MAIN_MENU,
    PLAYING,
    GAME_OVER
};

// ─── Globals (defined in Globals.cpp) ───────────────────────────────────────
extern std::vector<Entity*> entities;
extern std::list<Entity*>   toAddList;     // added after each frame
extern int        score;
extern int        lives;
extern int        highScore;
extern GameState  gameState;
