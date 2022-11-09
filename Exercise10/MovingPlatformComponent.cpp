//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#include "MovingPlatformComponent.hpp"
#include "PlatformComponent.hpp"
#include "GameObject.hpp"
#include <iostream>

MovingPlatformComponent::MovingPlatformComponent(GameObject *gameObject) : Component(gameObject)
{
    platformComponent = gameObject->getComponent<PlatformComponent>();
}

void MovingPlatformComponent::update(float deltaTime) {
    totalTime += deltaTime;

    float sin = glm::sin(totalTime);
    float v = glm::smoothstep(0.0f, 1.0f, sin);

    float sinRemap = 0 + (sin - -1) * (1 - 0) / (1 - -1);

    auto pos = glm::mix(movementStart, movementEnd, sinRemap);

    platformComponent->moveTo(pos);
}

void MovingPlatformComponent::setMovementStart(glm::vec2 pos) {
    movementStart = pos;
}

void MovingPlatformComponent::setMovementEnd(glm::vec2 pos) {
    movementEnd = pos;
}
