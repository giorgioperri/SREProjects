#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include <vector>

class GameObject;
class EnemyShip;
class SpaceShip;

class AsteroidsGame {
public:
    explicit AsteroidsGame(int playerAmount = 1);
    ~AsteroidsGame();
    static std::vector<std::shared_ptr<GameObject>> gameObjects;
    static std::shared_ptr<sre::SpriteAtlas> atlas;
private:
    int score;
    int players = 1;
    bool debugCollisionCircles = false;
    bool hasLost = false;
    bool isRunning = true;
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);
    void endGame();
    void initObjects();
    sre::SDLRenderer r;
    sre::Camera camera;
    std::vector<sre::Sprite> backgroundStars;
    std::vector<std::shared_ptr<SpaceShip>> allyShips;
    std::vector<std::shared_ptr<EnemyShip>> enemyShips;
};



