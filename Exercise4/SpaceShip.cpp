//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "SpaceShip.hpp"
#include "sre/Renderer.hpp"

SpaceShip::SpaceShip(const sre::Sprite &sprite) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    radius = 23;
    position = winSize * 0.5f;
    velocity = glm::vec2(0.0f,0.0f);
}

void SpaceShip::update(float deltaTime) {
    if (thrust && !isDead){
        float acceleration = deltaTime*thrustPower;
        glm::vec2 direction = glm::rotateZ(glm::vec3(0,acceleration,0), glm::radians(rotation));
        velocity += direction;
        float speed = glm::length(velocity);
        if (speed > maxSpeed){
            velocity = velocity * (maxSpeed / speed);
        }
    } else if (isDead) {
        rotation += deltaTime * rotationSpeed;
        velocity = velocity * (1.0f - drag*deltaTime);
    } else {
        velocity = velocity * (1.0f - drag*deltaTime);
    }
    position += velocity * deltaTime;
    if (rotateCCW){
        rotation += deltaTime * rotationSpeed;
    }
    if (rotateCW){
        rotation -= deltaTime * rotationSpeed;
    }

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

void SpaceShip::onCollision(std::shared_ptr<GameObject> other) {
    if (isDead) return;
    if(std::dynamic_pointer_cast<Laser>(other)) return;
    destroySelf();
}

void SpaceShip::fire() {
    auto meteorBigSprite = AsteroidsGame::atlas->get("laserBlue01.png");
    AsteroidsGame::gameObjects.push_back(std::make_shared<Laser>(meteorBigSprite, position, rotation));
}

void SpaceShip::onKey(SDL_Event &keyEvent) {
    if (isDead) {
        if(keyEvent.key.keysym.sym == SDLK_SPACE && keyEvent.type == SDL_KEYUP) {
            restart();
        }
        return;
    }

    if (keyEvent.key.keysym.sym == SDLK_UP){
        thrust = keyEvent.type == SDL_KEYDOWN;
    }
    if (keyEvent.key.keysym.sym == SDLK_LEFT){
        rotateCCW = keyEvent.type == SDL_KEYDOWN;
    }
    if (keyEvent.key.keysym.sym == SDLK_RIGHT){
        rotateCW = keyEvent.type == SDL_KEYDOWN;
    }
    if(keyEvent.key.keysym.sym == SDLK_SPACE) {
        if(keyEvent.type == SDL_KEYUP) {
            fire();
        }
    }
}

void SpaceShip::destroySelf() {
    isDead = true;
    sprite = AsteroidsGame::atlas->get("bang.png");
}

void SpaceShip::restart() {
    isDead = false;
    sprite = AsteroidsGame::atlas->get("playerShip1_orange.png");
    position = winSize * 0.5f;
    AsteroidsGame::initObjects();
}
