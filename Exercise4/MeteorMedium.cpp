//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "MeteorMedium.hpp"
#include "MeteorSmall.hpp"
#include "MeteorBig.hpp"
#include "Laser.hpp"
#include "sre/Renderer.hpp"

MeteorMedium::MeteorMedium(const sre::Sprite &sprite, glm::vec2 pos) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    radius = 13;

    position = pos;

    velocity = glm::vec2(rand() % 80,rand() % 80);
    rotationSpeed = rand() % 80;
}

void MeteorMedium::update(float deltaTime) {
    position += velocity * deltaTime;
    rotation += deltaTime * rotationSpeed;

    // wrap around
    if (position.x < 0){
        position.x += winSize.x;
    } else if (position.x > winSize.x){
        position.x -= winSize.x;
    }
    if (position.y < 0){
        position.y += winSize.y;
    } else if (position.y > winSize.y){
        position.y -= winSize.y;
    }
}

void MeteorMedium::onCollision(std::shared_ptr<GameObject> other) {

}

void MeteorMedium::destroyAndSpawn() {
    auto meteorIter = std::find_if(AsteroidsGame::gameObjects.begin(), AsteroidsGame::gameObjects.end(),
                                   [&](auto &s){ return s.get() == this; }
    );
    if (meteorIter != AsteroidsGame::gameObjects.end())
        AsteroidsGame::gameObjects.erase(meteorIter);

    auto meteorSmallSprite = AsteroidsGame::atlas->get("meteorBrown_small1.png");
    AsteroidsGame::gameObjects.push_back(std::make_shared<MeteorSmall>(meteorSmallSprite, position));
    AsteroidsGame::gameObjects.push_back(std::make_shared<MeteorSmall>(meteorSmallSprite, position));
}
