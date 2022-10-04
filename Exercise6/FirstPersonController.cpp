//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include "FirstPersonController.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "glm/gtx/euler_angles.hpp"
#include <iostream>

using namespace sre;
using namespace glm;


FirstPersonController::FirstPersonController(sre::Camera * camera)
:camera(camera)
{
    camera->setPerspectiveProjection(45,0.1f,1000);
    position = vec3(0,0,0);
}


void FirstPersonController::update(float deltaTime){

    auto currentView = camera->getViewTransform();

    mat4 translation = mat4(1.0f);

    if (fwd) {
        translation = translation * glm::translate( vec3(0,0,speed * deltaTime));
    }
    if (bwd) {
        translation = translation *  glm::translate(vec3(0,0,-speed * deltaTime));
    }
    if (left) {
        translation = translation *  glm::translate(vec3(speed * deltaTime,0,0));
    }
    if (right) {
        translation = translation *  glm::translate(vec3(-speed * deltaTime,0,0));
    }

    glm::mat4 rot = glm::eulerAngleY(glm::radians(deltaRotation));

    glm::mat4 newView =
            rot *
            translation *
            currentView;

    camera->setViewTransform(newView);

    vec4 newPos = translation * vec4(position,1);

    position = vec3(newPos.x, newPos.y, newPos.z);

    deltaRotation = 0;
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
    float offset = event.motion.xrel / mSens;
    rotation += offset;
    deltaRotation += offset;
}

void FirstPersonController::setInitialPosition(glm::vec2 position, float rotation) {
    this->position = glm::vec3(position.x,0,position.y);
    this->rotation = rotation;
}
