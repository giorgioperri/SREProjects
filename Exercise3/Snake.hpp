#pragma once

#include <memory>

class SnakeBody;

class Body;

class Snake {
public:
    Snake();

    void init(int initialLength, int x, int y, int moveX, int moveY);

    void move();

    void grow();

    // do not allow the movedirection to be the opposite of the current direction
    void setMoveDirection(int moveX, int moveY);

    bool collide();
private:
    //Body* body = nullptr;
    std::shared_ptr<Body> body = nullptr;
    //SnakeBody* snakeHead = nullptr;
    std::shared_ptr<SnakeBody> snakeHead = nullptr;
    int moveX = 1;
    int moveY = 0;
    int length;
    friend class SnakeGUI;
};

