//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include "FirstPersonController.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

using namespace sre;
using namespace glm;


FirstPersonController::FirstPersonController(sre::Camera * camera)
:camera(camera)
{
    camera->setPerspectiveProjection(45,0.1f,1000);
    position = vec3(0,0,0);
}


void FirstPersonController::update(float deltaTime){
    // todo implement
    camera->lookAt(position, position + vec3(0,0,-1), vec3{0,1,0});
    camera->setPositionAndRotation(position, glm::vec3(0,rotation,0));

    if (fwd) {
        position.z -= speed * deltaTime;
    }
    if (bwd) {
        position.z += speed * deltaTime;
    }
    if (left) {
        position.x -= speed * deltaTime;
    }
    if (right) {
        position.x += speed * deltaTime;
    }
}

void FirstPersonController::onKey(SDL_Event &event) {
    // Todo implement
    bool valueToSet = false;

    switch (event.type){
        case SDL_KEYDOWN:
            valueToSet = true;
            break;
        case SDL_KEYUP:
            valueToSet = false;
            break;
    }

    if(event.key.keysym.sym == SDLK_w) {
        fwd = valueToSet;
    }
    if(event.key.keysym.sym == SDLK_s) {
        bwd = valueToSet;
    }
    if(event.key.keysym.sym == SDLK_a) {
        left = valueToSet;
    }
    if(event.key.keysym.sym == SDLK_d) {
        right = valueToSet;
    }
}

void FirstPersonController::onMouse(SDL_Event &event) {
    // Todo implement
    rotation -= event.motion.xrel / mSens;

}

void FirstPersonController::setInitialPosition(glm::vec2 position, float rotation) {
    this->position = glm::vec3(position.x,0,position.y);
    this->rotation = rotation;
}
