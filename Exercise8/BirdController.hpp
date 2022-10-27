#pragma once

#include "Component.hpp"

class BirdController : public Component {
public:
    explicit BirdController(GameObject *gameObject);

    bool onKey(SDL_Event &event, std::shared_ptr<PhysicsComponent> comp = nullptr) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;
private:
};
