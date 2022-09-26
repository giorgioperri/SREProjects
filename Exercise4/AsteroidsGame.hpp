#pragma once

#include <vector>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"

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
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);
    sre::Camera camera;
    sre::SDLRenderer r;
    bool debugCollisionCircles = false;
    bool isRunning = true;
    int score;
    void initObjects();
    void endGame();
    std::vector<sre::Sprite> backgroundStars;
    int players = 1;
    std::vector<std::shared_ptr<SpaceShip>> allyShips;
    std::vector<std::shared_ptr<EnemyShip>> enemyShips;
};



