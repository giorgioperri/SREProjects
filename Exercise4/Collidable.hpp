#pragma once
#include "GameObject.hpp"

class Collidable {
public:
    virtual void onCollision(std::shared_ptr<GameObject> other) = 0;
    float getRadius() const;
protected:
    float radius = 0;
};


