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

    vec3 translationVector = vec3(0);

    if (fwd) {
        translationVector += vec3(0,0,-(1 + speed) * deltaTime);
    }
    if (bwd) {
        translationVector += vec3(0,0,(1 + speed) * deltaTime);
    }
    if (left) {
        translationVector += vec3(-(1 + speed) * deltaTime,0,0);
    }
    if (right) {
        translationVector += vec3((1 + speed) * deltaTime,0,0);
    }

    rotation += deltaRotation;

    //creating the rotation matrix
    mat4 rotationMatrix = glm::eulerAngleY(radians(rotation));

    //applying rotation to the translation vector
    translationVector = vec3(rotationMatrix * vec4(translationVector,1));

    //creating the result matrix of the rotation * translation
    mat4 translationMatrix = translate(translationVector);

    //assigning the result matrix to position
    position = vec3(translationMatrix * vec4(position,1));

    //applying rotation to the forward vector vec3(0,0,-1)
    vec3 rotatedForward = vec3(rotationMatrix * vec4(0,0,-1,1));

    //rotating the camera to look at the edited forward vector3
    camera->lookAt(position, position + rotatedForward, vec3(0,1,0));

    deltaRotation = 0;
}

void FirstPersonController::onKey(SDL_Event &event) {
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
    deltaRotation = event.motion.xrel / mSens;
}

void FirstPersonController::setInitialPosition(glm::vec2 position, float rotation) {
    this->position = glm::vec3(position.x,0,position.y);
    this->rotation = rotation;
}


