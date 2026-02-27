#include "Game.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Player.h"
#include "Collision.h"
#include "SoundManager.h"
#include "Global.h"

#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

// ─── Static members ───────────────────────────────────────────────────────────
sf::Font Game::font;
int      Game::currentLevel = 1;

// ─── Scoring ──────────────────────────────────────────────────────────────────
static int scoreForSize(AsteroidSize s) {
    switch (s) {
        case AsteroidSize::LARGE:  return 20;
        case AsteroidSize::MEDIUM: return 50;
        case AsteroidSize::SMALL:  return 100;
    }
    return 0;
}

// ─── Init ─────────────────────────────────────────────────────────────────────
bool Game::init() {
    SoundManager::instance();

    std::vector<std::string> fontPaths = {
        "assets/font.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/consola.ttf",
        "C:/Windows/Fonts/lucon.ttf",
        "/usr/share/fonts/truetype/freefont/FreeMono.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/System/Library/Fonts/Helvetica.ttf"
    };
    bool fontOk = false;
    for (auto& p : fontPaths)
        if (font.loadFromFile(p)) { fontOk = true; break; }

    loadHighScore();
    return fontOk;
}

// ─── Begin / restart ──────────────────────────────────────────────────────────
void Game::begin() {
    clearEntities();

    score        = 0;
    lives        = 3;
    currentLevel = 1;
    gameState    = GameState::PLAYING;

    entities.push_back(new Player());

    // Level 1 starts with 2 large asteroids.
    // Each subsequent level adds one more: Level 2 → 3, Level 3 → 4, etc.
    int count = 1 + currentLevel;   // Level 1 = 2, Level 2 = 3, …
    for (int i = 0; i < count; ++i)
        entities.push_back(new Asteroid(AsteroidSize::LARGE));
}

// ─── Main update ──────────────────────────────────────────────────────────────
void Game::update(sf::RenderWindow& window, float deltaTime) {
    window.clear(sf::Color::Black);
    switch (gameState) {
        case GameState::MAIN_MENU: updateMainMenu(window);            break;
        case GameState::PLAYING:   updatePlaying (window, deltaTime); break;
        case GameState::GAME_OVER: updateGameOver(window);            break;
    }
    window.display();
}

// ─── Event handler ────────────────────────────────────────────────────────────
void Game::handleEvent(const sf::Event& e) {
    if (e.type != sf::Event::KeyPressed) return;
    switch (gameState) {
        case GameState::MAIN_MENU:
            if (e.key.code == sf::Keyboard::Return) begin();
            break;
        case GameState::PLAYING:
            break;
        case GameState::GAME_OVER:
            if (e.key.code == sf::Keyboard::Return) begin();
            if (e.key.code == sf::Keyboard::Escape) {
                clearEntities();
                gameState = GameState::MAIN_MENU;
            }
            break;
    }
}

// ─── Main menu ────────────────────────────────────────────────────────────────
void Game::updateMainMenu(sf::RenderWindow& window) {
    drawCentredText(window, "ASTEROIDS", 96, 0.22f, sf::Color::White);
    drawCentredText(window, "PRESS  ENTER  TO  PLAY", 36, 0.50f, sf::Color(200,200,200));
    drawCentredText(window, "WASD / ARROWS  :  Move & Rotate", 24, 0.62f, sf::Color(150,150,150));
    drawCentredText(window, "SPACE  :  Fire", 24, 0.68f, sf::Color(150,150,150));
    if (highScore > 0)
        drawCentredText(window, "HIGH SCORE  " + std::to_string(highScore),
                        30, 0.82f, sf::Color(255,200,50));
}

// ─── Gameplay frame ───────────────────────────────────────────────────────────
void Game::updatePlaying(sf::RenderWindow& window, float deltaTime) {
    if (deltaTime > 0.1f) deltaTime = 0.1f;

    // 1. Update
    for (auto* e : entities)
        if (e->alive) e->update(deltaTime);

    // 2. Render
    for (auto* e : entities)
        if (e->alive) e->render(window);

    // 3. Collisions – may mark entities dead and push children to toAddList
    checkCollisions();

    // 4. Remove dead entities, then flush newly-created ones into the main list.
    //    Order matters: prune first so the level-advance check below sees
    //    accurate counts.
    pruneDeadEntities();
    flushAddList();

    // 5. Game-over check (must come after prune so lives are accurate)
    if (lives <= 0) {
        if (score > highScore) {
            highScore = score;
            saveHighScore();
        }
        gameState = GameState::GAME_OVER;
        return;
    }

    // 6. Level-advance check: runs AFTER prune+flush so we see the real state.
    //    anyAsteroidsAlive() only returns true for live Asteroid objects in
    //    entities – toAddList is already flushed in step 4.
    if (!anyAsteroidsAlive()) {
        ++currentLevel;
        int count = 1 + currentLevel;   // Level 2 → 3, Level 3 → 4, …
        for (int i = 0; i < count; ++i)
            entities.push_back(new Asteroid(AsteroidSize::LARGE));
    }

    drawHUD(window);
}

// ─── Game over ────────────────────────────────────────────────────────────────
void Game::updateGameOver(sf::RenderWindow& window) {
    drawCentredText(window, "GAME  OVER", 80, 0.20f, sf::Color(255, 60, 60));
    drawCentredText(window, "SCORE  " + std::to_string(score), 44, 0.40f);

    sf::Color hsCol = (score >= highScore && score > 0)
                    ? sf::Color(255,200,50) : sf::Color(180,180,180);
    drawCentredText(window, "HIGH SCORE  " + std::to_string(highScore),
                    36, 0.50f, hsCol);
    if (score >= highScore && score > 0)
        drawCentredText(window, "NEW  BEST!", 28, 0.59f, sf::Color(255,200,50));

    drawCentredText(window, "ENTER  :  Play Again", 28, 0.72f, sf::Color(200,200,200));
    drawCentredText(window, "ESCAPE  :  Main Menu", 28, 0.79f, sf::Color(200,200,200));
}

// ─── Collision detection ──────────────────────────────────────────────────────
void Game::checkCollisions() {
    Player*               player    = nullptr;
    std::vector<Bullet*>  bullets;
    std::vector<Asteroid*>asteroids;

    for (auto* e : entities) {
        if (!e->alive) continue;
        if (auto* p = dynamic_cast<Player*>  (e)) player = p;
        if (auto* b = dynamic_cast<Bullet*>  (e)) bullets.push_back(b);
        if (auto* a = dynamic_cast<Asteroid*>(e)) asteroids.push_back(a);
    }

    // Bullet vs Asteroid
    for (auto* bullet : bullets) {
        if (!bullet->alive) continue;
        for (auto* asteroid : asteroids) {
            if (!asteroid->alive) continue;
            if (Collision::circleIntersectsPolygon(
                    bullet->position, bullet->getRadius(),
                    asteroid->getWorldVertices()))
            {
                score += scoreForSize(asteroid->getSize());

                switch (asteroid->getSize()) {
                    case AsteroidSize::LARGE:
                        SoundManager::instance().play(SoundManager::Sound::EXPLODE_LARGE);
                        toAddList.push_back(new Asteroid(AsteroidSize::MEDIUM,
                            Asteroid::getRandomDirection(), asteroid->position));
                        toAddList.push_back(new Asteroid(AsteroidSize::MEDIUM,
                            Asteroid::getRandomDirection(), asteroid->position));
                        break;
                    case AsteroidSize::MEDIUM:
                        SoundManager::instance().play(SoundManager::Sound::EXPLODE_MEDIUM);
                        toAddList.push_back(new Asteroid(AsteroidSize::SMALL,
                            Asteroid::getRandomDirection(), asteroid->position));
                        toAddList.push_back(new Asteroid(AsteroidSize::SMALL,
                            Asteroid::getRandomDirection(), asteroid->position));
                        break;
                    case AsteroidSize::SMALL:
                        SoundManager::instance().play(SoundManager::Sound::EXPLODE_SMALL);
                        break; // no children
                }

                asteroid->alive = false;
                bullet->alive   = false;
                break; // bullet is spent
            }
        }
    }

    // Player vs Asteroid
    if (player && !player->isInvincible()) {
        auto playerVerts = player->getWorldVertices();
        for (auto* asteroid : asteroids) {
            if (!asteroid->alive) continue;
            if (Collision::polygonsIntersect(playerVerts,
                                             asteroid->getWorldVertices()))
            {
                SoundManager::instance().play(SoundManager::Sound::PLAYER_DIE);
                --lives;
                player->onHit();
                break;
            }
        }
    }
}

// ─── Level-advance helper ─────────────────────────────────────────────────────
bool Game::anyAsteroidsAlive() {
    for (auto* e : entities)
        if (e->alive && dynamic_cast<Asteroid*>(e)) return true;
    return false;
}

// ─── HUD ─────────────────────────────────────────────────────────────────────
void Game::drawHUD(sf::RenderWindow& window) {
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString(std::to_string(score));
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(30.f, 20.f);
    window.draw(scoreText);

    sf::Text levelText;
    levelText.setFont(font);
    levelText.setString("LEVEL  " + std::to_string(currentLevel));
    levelText.setCharacterSize(28);
    levelText.setFillColor(sf::Color(160, 160, 160));
    auto lb = levelText.getLocalBounds();
    levelText.setPosition((SCREEN_WIDTH - lb.width) / 2.f, 24.f);
    window.draw(levelText);

    drawLives(window);
}

void Game::drawLives(sf::RenderWindow& window) {
    const float SCALE  = 0.55f;
    const float MARGIN = 40.f;
    const float GAP    = 36.f;
    float startX = SCREEN_WIDTH - MARGIN - (lives - 1) * GAP;

    for (int i = 0; i < lives; ++i) {
        sf::VertexArray icon(sf::LinesStrip, 5);
        float cx = startX + i * GAP, cy = 40.f;
        auto sv = [&](int idx, float lx, float ly) {
            icon[idx].position = { cx + lx * SCALE, cy + ly * SCALE };
            icon[idx].color    = sf::Color::White;
        };
        sv(0,  20,  0); sv(1, -30, -20);
        sv(2, -15,  0); sv(3, -30,  20);
        sv(4,  20,  0);
        window.draw(icon);
    }
}

void Game::drawCentredText(sf::RenderWindow& window, const std::string& str,
                           unsigned size, float yFraction, sf::Color colour)
{
    sf::Text t;
    t.setFont(font);
    t.setString(str);
    t.setCharacterSize(size);
    t.setFillColor(colour);
    auto b = t.getLocalBounds();
    t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    t.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT * yFraction);
    window.draw(t);
}

// ─── Entity management ────────────────────────────────────────────────────────
void Game::pruneDeadEntities() {
    auto it = entities.begin();
    while (it != entities.end()) {
        if (!(*it)->alive) { delete *it; it = entities.erase(it); }
        else ++it;
    }
}

void Game::flushAddList() {
    for (auto* e : toAddList) entities.push_back(e);
    toAddList.clear();
}

void Game::clearEntities() {
    for (auto* e : entities)  delete e;  entities.clear();
    for (auto* e : toAddList) delete e;  toAddList.clear();
}

// ─── High score ───────────────────────────────────────────────────────────────
void Game::loadHighScore() {
    std::ifstream f("highscore.txt");
    if (f.is_open()) f >> highScore;
}

void Game::saveHighScore() {
    std::ofstream f("highscore.txt");
    if (f.is_open()) f << highScore;
}