#include <sre/Inspector.hpp>
#include <iostream>
#include "BirdGame.hpp"
#include "GameObject.hpp"
#include "sre/RenderPass.hpp"
#include "SpriteComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "PhysicsComponent.hpp"
#include "BirdController.hpp"

using namespace std;
using namespace sre;

const glm::vec2 BirdGame::windowSize(400,600);

BirdGame* BirdGame::instance = nullptr;

BirdGame::BirdGame()
        :debugDraw(physicsScale)
{
    instance = this;
    r.setWindowSize(windowSize);
    r.init()
      .withSdlInitFlags(SDL_INIT_EVERYTHING)
      .withSdlWindowFlags(SDL_WINDOW_OPENGL);

    init();

    // setup callback functions
    r.keyEvent = [&](SDL_Event& e){
        onKey(e);
    };
    r.frameUpdate = [&](float deltaTime){
        update(deltaTime);
    };
    r.frameRender = [&](){
        render();
    };
    // start game loop
    r.startEventLoop();
}

void BirdGame::init() {
    if (world != nullptr){ // deregister call backlistener to avoid getting callbacks when recreating the world
        world->SetContactListener(nullptr);
    }
    camera.reset();
    sceneObjects.clear();
    camera.reset();
    physicsComponentLookup.clear();
    initPhysics();
    auto camObj = createGameObject();
    camObj->name = "Camera";
    camera = camObj->addComponent<SideScrollingCamera>();
    camObj->setPosition(windowSize*0.5f);

    spriteAtlas = SpriteAtlas::create("bird.json","bird.png");

    auto birdObj = createGameObject();
    birdObj->name = "Bird";
    camera->setFollowObject(birdObj, {+150,BirdGame::windowSize.y/2});
    auto so = birdObj->addComponent<SpriteComponent>();
    auto sprite = spriteAtlas->get("bird1.png");
    sprite.setScale({2,2});

    birdObj->setPosition({-100,300});
    so->setSprite(sprite);
    auto anim = birdObj->addComponent<SpriteAnimationComponent>();
    auto phys = birdObj->addComponent<PhysicsComponent>();
    phys->initCircle(b2_dynamicBody, 10/physicsScale, {birdObj->getPosition().x/physicsScale,birdObj->getPosition().y/physicsScale}, 1);
    auto birdC = birdObj->addComponent<BirdController>();

    vector<Sprite> spriteAnim({spriteAtlas->get("bird1.png"),spriteAtlas->get("bird2.png"),spriteAtlas->get("bird3.png"),spriteAtlas->get("bird2.png")});
    for(auto & s : spriteAnim){
        s.setScale({2,2});
    }
    anim-> setSprites(spriteAnim);

    auto spriteBottom = spriteAtlas->get("column_bottom.png");
    spriteBottom.setScale({2,2});

    auto coinSprite = spriteAtlas->get("coin.png");
    coinSprite.setScale({1.3,1.3});

    float curve = 250;
    int length = 50;
    float heighVariation = 80;
    float xVariation = 50;

    for (int i=0;i<length;i++) {
        auto obj = createGameObject();
        obj->name = "Wall bottom";

        auto coinObj = createGameObject();
        coinObj->name = "Coin";

        auto so = obj->addComponent<SpriteComponent>();
        auto spriteComp = coinObj->addComponent<SpriteComponent>();

        float xOffset = xVariation * cos(i*curve*0.2f);

        glm::vec2 pos{i*300+xOffset,spriteBottom.getSpriteSize().y/2 + sin(i*curve)*heighVariation};
        obj->setPosition(pos);
        coinObj->setPosition({pos.x, pos.y + 220});

        auto phys = obj->addComponent<PhysicsComponent>();
        phys->initBox(b2_staticBody,glm::vec2(25/physicsScale,160/physicsScale), glm::vec2(obj->getPosition().x/physicsScale, obj->getPosition().y/physicsScale), 1);

        auto coinPhys = coinObj->addComponent<PhysicsComponent>();
        coinPhys->initCircle(b2_staticBody, 12/physicsScale, {coinObj->getPosition()/physicsScale}, 1);
        coinPhys->setSensor(true);

        so->setSprite(spriteBottom);
        spriteComp->setSprite(coinSprite);

        glm::vec2 s { spriteBottom.getSpriteSize().x * spriteBottom.getScale().x/2, spriteBottom.getSpriteSize().y * spriteBottom.getScale().y/2};
    }

    auto spriteTop = spriteAtlas->get("column_top.png");
    spriteTop.setScale({2,2});
    for (int i=0;i<length;i++){
        auto obj = createGameObject();
        obj->name = "Wall top";
        auto so = obj->addComponent<SpriteComponent>();

        float xOffset = xVariation * cos(i*curve*0.2f);
        glm::vec2 pos{ i*300+xOffset, windowSize.y-spriteTop.getSpriteSize().y/2 + sin(i*curve)*heighVariation};
        obj->setPosition(pos);

        auto phys = obj->addComponent<PhysicsComponent>();
        phys->initBox(b2_staticBody,glm::vec2(25/physicsScale,160/physicsScale), glm::vec2(obj->getPosition().x/physicsScale, obj->getPosition().y/physicsScale), 1);

        glm::vec2 s { spriteTop.getSpriteSize().x * spriteTop.getScale().x/2, spriteTop.getSpriteSize().y * spriteTop.getScale().y/2};
        so->setSprite(spriteTop);
    }

    auto floorCollider = createGameObject();
    floorCollider->name = "Floor";
    floorCollider->setPosition({birdObj->getPosition().x + 100,130});
    auto floorPhysics = floorCollider->addComponent<PhysicsComponent>();
    floorPhysics->initBox(b2_kinematicBody,glm::vec2(250/physicsScale,1/physicsScale), glm::vec2(floorCollider->getPosition().x/physicsScale, floorCollider->getPosition().y/physicsScale), 1);
    floorPhysics->setLinearVelocity(glm::vec2(phys->getLinearVelocity().x,0));

    auto ceilCollider = createGameObject();
    ceilCollider->name = "Ceiling";
    ceilCollider->setPosition({birdObj->getPosition().x + 100,600});
    auto ceilPhysics = ceilCollider->addComponent<PhysicsComponent>();
    ceilPhysics->initBox(b2_kinematicBody,glm::vec2(250/physicsScale,1/physicsScale), glm::vec2(ceilCollider->getPosition().x/physicsScale, ceilCollider->getPosition().y/physicsScale), 1);
    ceilPhysics->setLinearVelocity(glm::vec2(phys->getLinearVelocity().x,0));

    background1Component.init("background.png");
    background2Component.init("background2.png");
}

void BirdGame::update(float time) {
    if (gameState == GameState::Running){
        updatePhysics();
    }
    for (int i=0;i<sceneObjects.size();i++){
        for (auto & c : sceneObjects[i]->getComponents()){
            if(dynamic_pointer_cast<BirdController>(c) != nullptr && dynamic_pointer_cast<BirdController>(c)->isDead) {
                gameState = GameState::GameOver;
            }
        }
        
        if(sceneObjects[i]->queueForRemoval) {
            sceneObjects.erase(std::remove(sceneObjects.begin(), sceneObjects.end(), sceneObjects[i]), sceneObjects.end());
        }

        sceneObjects[i]->update(time);

    }
}

void BirdGame::render() {
    auto rp = RenderPass::create()
            .withCamera(camera->getCamera())
            .build();

    auto pos = camera->getGameObject()->getPosition();
    background1Component.renderBackground(rp, +pos.x*0.8f);

    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto & go : sceneObjects){
        go->renderSprite(spriteBatchBuilder);
    }

    if (gameState == GameState::Ready){
        auto sprite = spriteAtlas->get("get-ready.png");
        sprite.setPosition(pos);
        spriteBatchBuilder.addSprite(sprite);
    } else if (gameState == GameState::GameOver){
        auto sprite = spriteAtlas->get("game-over.png");
        sprite.setPosition(pos);
        spriteBatchBuilder.addSprite(sprite);
    }

    auto sb = spriteBatchBuilder.build();
    rp.draw(sb);

    background2Component.renderBackground(rp, -pos.x*0.2f);

    if (doDebugDraw){
        world->DrawDebugData();
        rp.drawLines(debugDraw.getLines());
        debugDraw.clear();
    }
}

void BirdGame::onKey(SDL_Event &event) {
    for (auto & gameObject: sceneObjects) {
        for (auto & c : gameObject->getComponents()){
            if(dynamic_pointer_cast<BirdController>(c) != nullptr) {
                bool consumed = c->onKey(event, c->getGameObject()->getComponent<PhysicsComponent>());
            }
            bool consumed = c->onKey(event);
            if (consumed){
                return;
            }
        }
    }

    if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym){
            case SDLK_d:
                // press 'd' for physics debug
                doDebugDraw = !doDebugDraw;
                if (doDebugDraw){
                    world->SetDebugDraw(&debugDraw);
                } else {
                    world->SetDebugDraw(nullptr);
                }
                break;
            case SDLK_r:
                init();
                break;
            case SDLK_SPACE:
                if (gameState == GameState::GameOver){
                    init();
                    gameState = GameState::Ready;
                } else if (gameState == GameState::Ready){
                    gameState = GameState::Running;
                }
                break;

        }
    }
}

std::shared_ptr<GameObject> BirdGame::createGameObject() {
    auto obj = shared_ptr<GameObject>(new GameObject());
    sceneObjects.push_back(obj);
    return obj;
}

void BirdGame::updatePhysics() {
    const float32 timeStep = 1.0f / 60.0f;
    const int positionIterations = 2;
    const int velocityIterations = 6;
    world->Step(timeStep, velocityIterations, positionIterations);

    for (auto phys : physicsComponentLookup){
        if (phys.second->rbType == b2_staticBody) continue;
        auto position = phys.second->body->GetPosition();
        float angle = phys.second->body->GetAngle();
        auto gameObject = phys.second->getGameObject();
        gameObject->setPosition(glm::vec2(position.x*physicsScale, position.y*physicsScale));
        gameObject->setRotation(angle);
    }
}

void BirdGame::initPhysics() {
    float gravity = -9.8; // 9.8 m/s2
    delete world;
    world = new b2World(b2Vec2(0,gravity));
    world->SetContactListener(this);

    if (doDebugDraw){
        world->SetDebugDraw(&debugDraw);
    }
}

void BirdGame::BeginContact(b2Contact *contact) {
    b2ContactListener::BeginContact(contact);
    handleContact(contact, true);
}

void BirdGame::EndContact(b2Contact *contact) {
    b2ContactListener::EndContact(contact);
    handleContact(contact, false);
}

void BirdGame::deregisterPhysicsComponent(PhysicsComponent *r) {
    auto iter = physicsComponentLookup.find(r->fixture);
    if (iter != physicsComponentLookup.end()){
        physicsComponentLookup.erase(iter);
    } else {
        assert(false); // cannot find physics object
    }
}

void BirdGame::registerPhysicsComponent(PhysicsComponent *r) {
    physicsComponentLookup[r->fixture] = r;
}

void BirdGame::handleContact(b2Contact *contact, bool begin) {
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto physA = physicsComponentLookup.find(fixA);
    auto physB = physicsComponentLookup.find(fixB);
    if (physA !=physicsComponentLookup.end() && physB != physicsComponentLookup.end()){
        auto & aComponents = physA->second->getGameObject()->getComponents();
        auto & bComponents = physB->second->getGameObject()->getComponents();
        for (auto & c : aComponents){
            if (begin){
                if (!physB->second->isSensor()) {
                    c->onCollisionStart(physB->second);
                } else {
                    physB->second->getGameObject()->queueForRemoval = true;
                }
            } else {
                c->onCollisionEnd(physB->second);
            }
        }
        for (auto & c : bComponents){
            if (begin){
                if (!physA->second->isSensor()) {
                    c->onCollisionStart(physA->second);
                } else {
                    physB->second->getGameObject()->queueForRemoval = true;
                }
            } else {
                c->onCollisionEnd(physA->second);
            }
        }
    }
}

void BirdGame::setGameState(GameState newState) {
    this->gameState = newState;
}
