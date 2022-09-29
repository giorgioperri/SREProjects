//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "sre/Renderer.hpp"
#include "SpaceShip.hpp"
#include "Meteor.hpp"
#include "Enemy.hpp"

Meteor::Meteor(const sre::Sprite &sprite, MeteorSize meteorSize, glm::vec2 pos) : GameObject(sprite) {
    winSize = sre::Renderer::instance->getDrawableSize();
    scale = glm::vec2(.5f,.5f);
    size = meteorSize;

    //calculate spawn position and radius based on meteor type
    switch (size) {
        case Big:
            float randX1, randX2, randY1, randY2;
            radius = 28;
            randX1 = rand() % (int)(winSize.x-(winSize.x * 0.6f) + 1) + winSize.x * 0.8f;
            randX2 = rand() % (int) winSize.x * 0.4f;
            randY1 = rand() % (int)(winSize.y-(winSize.y * 0.6f) + 1) + winSize.y * 0.8f;
            randY2 = rand() % (int) winSize.y * 0.4f;
            position.x = rand() % 50 > 25 ? randX1 : randX2;
            position.y = rand() % 50 > 25 ? randY1 : randY2;
            break;
        case Medium:
            radius = 14;
            position = pos;
            break;
        default:
            radius = 5;
            position = pos;
            break;
    }

    //random vel and rotation
    velocity = glm::vec2(rand() % 80,rand() % 80);
    rotationSpeed = rand() % 80;
}

void Meteor::update(float deltaTime) {
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

void Meteor::onCollision(std::shared_ptr<GameObject> other) {
    if(std::dynamic_pointer_cast<Meteor>(other) ||
            std::dynamic_pointer_cast<SpaceShip>(other) ||
            std::dynamic_pointer_cast<EnemyShip>(other)) return;

    if(std::dynamic_pointer_cast<Laser>(other) &&
            std::dynamic_pointer_cast<Laser>(other)->currentLaserType == Enemy) return;

    //if collision is accepted queue for removal and...
    queueForRemoval = true;

    sre::Sprite spriteToIssue;

    //...behave based on the size
    if(size == Big) {
        spriteToIssue = AsteroidsGame::atlas->get("meteorBrown_med1.png");
        AsteroidsGame::gameObjects.push_back(std::make_shared<Meteor>(spriteToIssue, Medium, position));
        AsteroidsGame::gameObjects.push_back(std::make_shared<Meteor>(spriteToIssue, Medium, position));
    } else if (size == Medium) {
        spriteToIssue = AsteroidsGame::atlas->get("meteorBrown_small1.png");
        AsteroidsGame::gameObjects.push_back(std::make_shared<Meteor>(spriteToIssue, Small, position));
        AsteroidsGame::gameObjects.push_back(std::make_shared<Meteor>(spriteToIssue, Small, position));
    }
}

Meteor::~Meteor() = default;