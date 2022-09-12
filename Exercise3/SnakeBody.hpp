#pragma once

#include "SnakeGUI.hpp"
#include <tuple>

class SnakeBody {
public:
    SnakeBody(int x, int y);
    ~SnakeBody();
    void moveTo(int x, int y);
    SnakeBody* next = nullptr;
    SnakeBody* prev = nullptr;
    int x;
    int y;
private:
    static int totalSnakeBodies;
    friend class Snake;
    friend class SnakeGUI;
};

class Body {
private:
    SnakeBody *head, *tail;
    int totalSnakeBodies;
public:
    Body() {
        head = nullptr;
    }

    ~Body() {
        --totalSnakeBodies;
        delete head;
        delete tail;
    }

    SnakeBody* addBodyPart(int x, int y) {
        SnakeBody *bodyPart = new SnakeBody(x, y);
        bodyPart-> next = nullptr;
        bodyPart-> prev = nullptr;

        if(head == nullptr) {
            head = bodyPart;
            tail = bodyPart;
        } else {
            tail->next = bodyPart;
            bodyPart->prev = tail;
            tail = tail->next;
        }

        ++totalSnakeBodies;

        return bodyPart;
    }

    void move(int moveX, int moveY) {
        int prevX = head->x;
        int prevY = head->y;

        head->x += moveX;
        head->y += moveY;

        auto bodyPart = head;

        while(bodyPart->next != nullptr) {
            bodyPart = bodyPart->next;

            int prevX2 = bodyPart->x;
            int prevY2 = bodyPart->y;

            bodyPart->moveTo(prevX, prevY);

            if(bodyPart->next == nullptr) break;

            bodyPart = bodyPart->next;

            prevX = bodyPart->x;
            prevY = bodyPart->y;

            bodyPart->moveTo(prevX2, prevY2);
        }
    }

    bool checkSelfCollision() {

        auto snakeHead = head;

        for (auto node = snakeHead; node != nullptr; node = node->next) {
            for (auto node2 = snakeHead; node2 != nullptr; node2 = node2->next) {
                if(node == node2) continue;
                if(node->x == node2->x && node->y == node2->y) {
                    return true;
                }
            }
        }

        return false;
    }

    int size() {
        return totalSnakeBodies;
    }

    int getTailX() {
        return tail->x;
    }

    int getTailY() {
        return tail->y;
    }
};

