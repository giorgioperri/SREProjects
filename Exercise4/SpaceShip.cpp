//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "sre/Renderer.hpp"
#include "SpaceShip.hpp"

SpaceShip::SpaceShip(const sre::Sprite &sprite, PlayerNumber pNum, glm::vec2 pos, float rot) : GameObject(sprite) {
    winSize = sre::Renderer::instance->getDrawableSize();
    velocity = glm::vec2(0.0f,0.0f);
    scale = glm::vec2(0.5f,0.5f);
    playerNumber = pNum;
    rotation = rot;
    position = pos;
    radius = 23;
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
        // funny behaviour on death
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
    if (isDead || std::dynamic_pointer_cast<SpaceShip>(other) ||
            std::dynamic_pointer_cast<Laser>(other)) return;
    destroySelf();
}

void SpaceShip::fire() {
    auto laserSprite = AsteroidsGame::atlas->get("laserBlue01.png");
    AsteroidsGame::gameObjects.push_back(std::make_shared<Laser>(laserSprite, position, rotation));
}

void SpaceShip::onKey(SDL_Event &keyEvent) {
    if (isDead) return;

    if ((playerNumber == PlayerOne && keyEvent.key.keysym.sym == SDLK_UP) || (playerNumber == PlayerTwo && keyEvent.key.keysym.sym == SDLK_w)){
        thrust = keyEvent.type == SDL_KEYDOWN;
    }
    if ((playerNumber == PlayerOne && keyEvent.key.keysym.sym == SDLK_LEFT) || (playerNumber == PlayerTwo && keyEvent.key.keysym.sym == SDLK_a)){
        rotateCCW = keyEvent.type == SDL_KEYDOWN;
    }
    if ((playerNumber == PlayerOne && keyEvent.key.keysym.sym == SDLK_RIGHT) || (playerNumber == PlayerTwo && keyEvent.key.keysym.sym == SDLK_d)){
        rotateCW = keyEvent.type == SDL_KEYDOWN;
    }

    //fire on space up
    if((playerNumber == PlayerOne && keyEvent.key.keysym.sym == SDLK_SPACE) || (playerNumber == PlayerTwo && keyEvent.key.keysym.sym == SDLK_e)) {
        if(keyEvent.type == SDL_KEYUP) {
            fire();
        }
    }
}

void SpaceShip::destroySelf() {
    //replace sprite on death
    isDead = true;
    sprite = AsteroidsGame::atlas->get("bang.png");
}

void SpaceShip::restart() {
    //reset death status, sprite and position
    isDead = false;
    sprite = AsteroidsGame::atlas->get("playerShip1_orange.png");
    position = winSize * 0.5f;
}

SpaceShip::~SpaceShip() = default;