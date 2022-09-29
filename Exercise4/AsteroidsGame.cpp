#include <ctime>
#include <glm/gtc/constants.hpp>
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "SpaceShip.hpp"
#include "Meteor.hpp"
#include "Enemy.hpp"

using namespace sre;

//declaring static fields
std::vector<std::shared_ptr<GameObject>> AsteroidsGame::gameObjects;
std::shared_ptr<sre::SpriteAtlas> AsteroidsGame::atlas;

AsteroidsGame::~AsteroidsGame() {}

//passing how many players are playing to the players
AsteroidsGame::AsteroidsGame(int playerAmount) {

    players = playerAmount;

    //clamping values
    if (players < 1) players = 1;
    if (players > 2) players = 2;

    r.setWindowTitle("Asteroids");

    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
            .withSdlWindowFlags(SDL_WINDOW_OPENGL);

    // random seed based on time
    time_t t;
    srand((unsigned) time(&t));

    atlas = SpriteAtlas::create("asteroids.json","asteroids.png");

    //creating all the needed objects
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
        //if the object should be destroyed this frame we can avoid executing the update
        if (gameObjects[i]->queueForRemoval) continue;
        gameObjects[i]->update(deltaTime);

        //stop the game if one SpaceShip died
        if(std::dynamic_pointer_cast<SpaceShip>(gameObjects[i])) {
            auto playerShip = std::dynamic_pointer_cast<SpaceShip>(gameObjects[i]);
            if(playerShip->isDead) {
                hasLost = true;
                isRunning = false;
            }
        }

        //update the enemy rotation to look at the corresponding player
        for (int k = 0; k < players; ++k) {
            if(allyShips.size() > 0 && enemyShips.size() > 0){
                float angle = atan2(enemyShips[k]->position.y - allyShips[k]->position.y,  enemyShips[k]->position.x - allyShips[k]->position.x);
                enemyShips[k]->rotation = (angle * 180 / 3.14f) + 90;
            }
        }

        //figure out collisions
        for (int j = 0; j < gameObjects.size();j++) {
            //if object is self continue
            if(gameObjects[i] == gameObjects[j]) continue;

            //all game objects derive from Collidable, so it's safe to assume the dynamic cast will work
            std::shared_ptr<Collidable> coll1 = std::dynamic_pointer_cast<Collidable> (gameObjects[i]);
            std::shared_ptr<Collidable> coll2 = std::dynamic_pointer_cast<Collidable> (gameObjects[j]);

            float rSum = coll1->getRadius() + coll2->getRadius();

            //Pythagoras gvng
            if ((gameObjects[i]->position.x - gameObjects[j]->position.x) * (gameObjects[i]->position.x - gameObjects[j]->position.x) +
                    (gameObjects[i]->position.y - gameObjects[j]->position.y)*(gameObjects[i]->position.y - gameObjects[j]->position.y) < rSum*rSum) {
                coll1->onCollision(gameObjects[j]);
                coll2->onCollision(gameObjects[i]);
            }
        }
    }

    //destroy all doomed objects
    for (int i = 0; i < gameObjects.size();i++) {
        if (gameObjects[i]->queueForRemoval) {
            if(!std::dynamic_pointer_cast<Laser>(gameObjects[i])) {
            score++;
            }
            gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObjects[i]), gameObjects.end());
        }
    }

    if(gameObjects.size() <= players) {
        endGame();
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

    //add stars to the batch
    for (int i = 0; i < backgroundStars.size();i++) {
        spriteBatchBuilder.addSprite(backgroundStars[i]);
    }

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
    ImGui::SetNextWindowSize(ImVec2(200, 60), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
    ImGui::LabelText("Score", "%i",score);
    ImGui::End();

    if (isRunning) return;

    // Win-lose UI
    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x/2-90, Renderer::instance->getWindowSize().y/2-25), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(180, 50), ImGuiSetCond_Always);
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::Text(hasLost ? "You Lose!" : "You Win!");
    ImGui::Text("Press SPACE to restart!");
    ImGui::End();
}

void AsteroidsGame::keyEvent(SDL_Event &event) {
    for (int i = 0; i < gameObjects.size();i++) {
        if(isRunning) gameObjects[i]->onKey(event);

        //space restarts if dead
        if(std::dynamic_pointer_cast<SpaceShip>(gameObjects[i])) {
            auto ship = std::dynamic_pointer_cast<SpaceShip>(gameObjects[i]);
            if((ship->isDead || !isRunning) && event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
                ship->restart();
                initObjects();
                isRunning = true;
            }
        }
    }

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r){
        debugCollisionCircles = !debugCollisionCircles;
    }
}

void AsteroidsGame::initObjects() {

    //at start, resets game

    hasLost = false;
    backgroundStars.clear();
    score = 0;
    gameObjects.clear();
    allyShips.clear();
    enemyShips.clear();

    auto winSize = sre::Renderer::instance->getDrawableSize();

    //adding stars to the background with random sprites, offsets and rotations
    for (int i = 50; i < winSize.x; i += winSize.x/6) {
        for (int j = 50; j < winSize.y; j += winSize.y/6) {
            sre::Sprite starSprite;
            const int randNum = rand() % 999;
            if (randNum <= 333) {
                starSprite = atlas->get("star1.png");
            } else if (randNum <= 666) {
                starSprite = atlas->get("star2.png");
            } else {
                starSprite = atlas->get("star3.png");
            }
            glm::vec2 position = glm::vec2(i + rand() % 81 + (-40), j + rand() % 81 + (-40));
            double scaleParam = (((float) rand() / RAND_MAX) * 0.5) + 0.1;
            glm::vec2 scale = glm::vec2(scaleParam, scaleParam);
            float rotation = 30;
            starSprite.setPosition(position);
            starSprite.setRotation(rotation);
            starSprite.setScale(scale);
            backgroundStars.push_back(starSprite);
        }
    }

    //creating the player1 ship
    auto spaceshipOneSprite = atlas->get("playerShip1_orange.png");
    auto playerShip = std::make_shared<SpaceShip>(spaceshipOneSprite, PlayerOne,
                                                  glm::vec2(sre::Renderer::instance->getDrawableSize().x / 2,
                                                            sre::Renderer::instance->getDrawableSize().y / 2 - 30));
    gameObjects.push_back(playerShip);
    allyShips.push_back(playerShip);

    //if multiplayer, creating another ship
    if(players > 1) {
        auto spaceshipTwoSprite = atlas->get("playerShip1_green.png");
        auto playerShip2 = std::make_shared<SpaceShip>(spaceshipTwoSprite, PlayerTwo,
                                                       glm::vec2(sre::Renderer::instance->getDrawableSize().x / 2,
                                                                 sre::Renderer::instance->getDrawableSize().y / 2 + 30));
        gameObjects.push_back(playerShip2);
        allyShips.push_back(playerShip2);
    }

    //creating one enemy for every player
    auto enemySprite = atlas->get("enemyBlack2.png");
    for (int i = 0; i < players; ++i) {
        auto enemyShip = std::make_shared<EnemyShip>(enemySprite);
        gameObjects.push_back(enemyShip);
        enemyShips.push_back(enemyShip);
    }

    //creating the asteroids
    auto meteorBigSprite = atlas->get("meteorBrown_big4.png");
    for (int i = 0; i < 5; ++i) {
        gameObjects.push_back(std::make_shared<Meteor>(meteorBigSprite, Big));
    }
}

void AsteroidsGame::endGame() {
    isRunning = false;
}

int main(){
    int players;
    std::cout << "How many people are going to play? (Type 1 or 2)\n";
    std::cin >> players;
    new AsteroidsGame(players);
    return 0;
}