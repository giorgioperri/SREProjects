//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "Meteor.hpp"
#include "SpaceShip.hpp"
#include "sre/Renderer.hpp"

Meteor::Meteor(const sre::Sprite &sprite, MeteorSize meteorSize, glm::vec2 pos) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    size = meteorSize;

    float randX1, randX2, randY1, randY2;

    switch (size) {
        case Big:
            radius = 28;
            randX1 = rand() % (int)(winSize.x-(winSize.x * 0.6f) + 1) + winSize.x * 0.8f;
            randX2 = rand() % (int) winSize.x * 0.4f;
            randY1 = rand() % (int)(winSize.y-(winSize.y * 0.6f) + 1) + winSize.y * 0.8f;
            randY2 = rand() % (int) winSize.y * 0.4f;

            this->position.x = rand() % 50 > 25 ? randX1 : randX2;
            this->position.y = rand() % 50 > 25 ? randY1 : randY2;
            break;
        case Medium:
            radius = 14;
            this->position = pos;
            break;
        default:
            radius = 5;
            this->position = pos;
            break;
    }

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
    if(std::dynamic_pointer_cast<Meteor>(other) || std::dynamic_pointer_cast<SpaceShip>(other)) return;
    queueForRemoval = true;

    sre::Sprite spriteToIssue;

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