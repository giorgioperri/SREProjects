//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <SDL_events.h>
#include <iostream>
#include "BirdController.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "BirdGame.hpp"
#include "SpriteComponent.hpp"

BirdController::BirdController(GameObject *gameObject) : Component(gameObject) {
    gameObject->getComponent<PhysicsComponent>()->setLinearVelocity(glm::vec2(1,0));
}

bool BirdController::onKey(SDL_Event &event, std::shared_ptr<PhysicsComponent> comp) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE){
        if(comp != nullptr) {
            comp->setLinearVelocity(glm::vec2(comp->getLinearVelocity().x, 0));
            comp->addImpulse(glm::vec2(0,.1));
        }

    } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE){
        std::cout << "some key released" << std::endl;
    }
    return false;
}

void BirdController::onCollisionStart(PhysicsComponent *comp) {
    std::cout << "bird collided with something" << std::endl;
    /* if is not coin */ isDead = true;
}

void BirdController::onCollisionEnd(PhysicsComponent *comp) {

}


