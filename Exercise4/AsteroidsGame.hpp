#pragma once

#include <vector>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"

class GameObject;

class AsteroidsGame {
public:
    AsteroidsGame();
    ~AsteroidsGame();
    static std::vector<std::shared_ptr<GameObject>> gameObjects;
    static std::shared_ptr<sre::SpriteAtlas> atlas;
    static void AddGameObject();
private:
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);

    sre::Camera camera;
    sre::SDLRenderer r;
    bool debugCollisionCircles = false;
    int score = 0;

    void fireLaser(glm::vec2 vec, float d);
};



