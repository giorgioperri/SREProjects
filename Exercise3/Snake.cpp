#include "Snake.hpp"
#include "SnakeBody.hpp"

Snake::Snake() {
}

void Snake::move(){

    body->move(moveX, moveY);

    // todo no opposite dir
}

void Snake::setMoveDirection(int moveX, int moveY) {                // This is called whenever one arrow key is pressed. Values are [-1,0,1] for x and y.
    if(this->moveX == -moveX || this->moveY == -moveY) return;
    this->moveX = moveX;
    this->moveY = moveY;
}

void Snake::init(int initialLength, int x, int y, int moveX, int moveY) {   // This is called every time the game starts or restarts. Make sure not to leak resources.
    length = initialLength;
    this->moveX = moveX;
    this->moveY = moveY;
    body = new Body();
    snakeHead = body->addBodyPart(x,y);

    for (int i = 1; i < length; ++i) {
        body->addBodyPart(x-i,y);
    }
}

void Snake::grow() {                                                // Should increment the length and append a SnakeBody
    ++length;
    body->addBodyPart(body->getTailX(), body->getTailY());
}

bool Snake::collide() {// Returns true if one of the SnakeBody objects has the position (x,y)
    return body->checkSelfCollision();
}
