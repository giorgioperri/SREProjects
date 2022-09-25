//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//

#include <ctime>
#include <glm/gtc/constants.hpp>
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "SpaceShip.hpp"
#include "Meteor.hpp"

using namespace sre;

std::shared_ptr<sre::SpriteAtlas> AsteroidsGame::atlas;
std::vector<std::shared_ptr<GameObject>> AsteroidsGame::gameObjects;
int AsteroidsGame::score = 0;

AsteroidsGame::~AsteroidsGame() {}

AsteroidsGame::AsteroidsGame() {
    r.setWindowTitle("Asteroids");

    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
            .withSdlWindowFlags(SDL_WINDOW_OPENGL);

    time_t t;                                               // random seed based on time
    srand((unsigned) time(&t));

    atlas = SpriteAtlas::create("asteroids.json","asteroids.png");

    initObjects();

    camera.setWindowCoordinates();

    r.frameUpdate = [&](float deltaTime){
        update(deltaTime);
    };

    r.keyEvent = [&](SDL_Event& event){
        keyEvent(event);
    };

    r.frameRender = [&](){
        render();
    };

    r.startEventLoop();
}

void AsteroidsGame::update(float deltaTime) {
    for (int i = 0; i < gameObjects.size();i++) {
        if (gameObjects[i]->queueForRemoval) continue;
        gameObjects[i]->update(deltaTime);

        for (int j = 0; j < gameObjects.size();j++) {
            if(gameObjects[i] == gameObjects[j]) continue;

            std::shared_ptr<Collidable> coll1 = std::dynamic_pointer_cast<Collidable> (gameObjects[i]);
            std::shared_ptr<Collidable> coll2 = std::dynamic_pointer_cast<Collidable> (gameObjects[j]);

            float x1 = gameObjects[i]->position.x;
            float x2 = gameObjects[j]->position.x;
            float y1 = gameObjects[i]->position.y;
            float y2 = gameObjects[j]->position.y;

            float rSum = coll1->getRadius() + coll2->getRadius();

            if ((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) < rSum*rSum) {
                coll1->onCollision(gameObjects[j]);
                coll2->onCollision(gameObjects[i]);
            }
        }
    }

    for (int i = 0; i < gameObjects.size();i++) {
        if (gameObjects[i]->queueForRemoval) {
            if(!std::dynamic_pointer_cast<Laser>(gameObjects[i])) {
            score++;
            }
            gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObjects[i]), gameObjects.end());
        }
    }
}

void drawCircle(std::vector<glm::vec3>& lines, glm::vec2 position, float radius){
    float quaterPi = glm::quarter_pi<float>();
    for (float f = 0;f<glm::two_pi<float>();f += quaterPi){
        // line from
        lines.push_back(glm::vec3{position.x + cosf(f)*radius,
                                  position.y + sinf(f)*radius,
                                  0
        });
        // line to
        lines.push_back(glm::vec3{position.x + cosf(f+quaterPi)*radius,
                                  position.y + sinf(f+quaterPi)*radius,
                                  0
        });
    }
}

void AsteroidsGame::render() {
    auto renderPass = RenderPass::create()
            .withCamera(camera)
            .withClearColor(true, {.20, .60, .86, 1})
            .build();
    auto spriteBatchBuilder = SpriteBatch::create();

    for (int i = 0; i < gameObjects.size();i++) {
        gameObjects[i]->render(spriteBatchBuilder);
    }
    auto spriteBatch = spriteBatchBuilder.build();
    renderPass.draw(spriteBatch);

    if (debugCollisionCircles){
        std::vector<glm::vec3> lines;
        for (auto & go : gameObjects){
            auto col = std::dynamic_pointer_cast<Collidable>(go);
            if (col != nullptr){
                drawCircle(lines, go->position, col->getRadius());
            }
        }
        renderPass.drawLines(lines);
    }

    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x/2-100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
    ImGui::LabelText("Score", "%i",score);
    ImGui::End();
}

void AsteroidsGame::keyEvent(SDL_Event &event) {
    for (int i = 0; i < gameObjects.size();i++) {
        gameObjects[i]->onKey(event);
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d){
        debugCollisionCircles = !debugCollisionCircles;
    }
}

void AsteroidsGame::initObjects() {
    score = 0;
    gameObjects.clear();

    auto spaceshipSprite = atlas->get("playerShip1_orange.png");
    gameObjects.push_back(std::make_shared<SpaceShip>(spaceshipSprite));

    auto meteorBigSprite = atlas->get("meteorBrown_big4.png");
    for (int i = 0; i < 5; ++i) {
        gameObjects.push_back(std::make_shared<Meteor>(meteorBigSprite, Big));
    }
}

int main(){
    new AsteroidsGame();
    return 0;
}