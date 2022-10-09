//
// Created by Morten Nobel-Jørgensen on 22/09/2017.
//

#include "Wolf3D.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>


using namespace std;
using namespace sre;
using namespace glm;

Wolf3D::Wolf3D()
:fpsController(&camera)
{
    r.init();
    init();

     //Enable mouse lock
     SDL_SetWindowGrab(r.getSDLWindow(),SDL_TRUE);
     SDL_SetRelativeMouseMode(SDL_TRUE);

    r.frameUpdate = [&](float deltaTime){
        update(deltaTime);
    };
    r.frameRender = [&](){
        render();
    };
    r.keyEvent = [&](SDL_Event& e){
        fpsController.onKey(e);
    };
    r.mouseEvent = [&](SDL_Event& e){
        if (!lockRotation){
            fpsController.onMouse(e);
        }
    };
    r.startEventLoop();
}

void Wolf3D::update(float deltaTime) {
    fpsController.update(deltaTime);
}

void Wolf3D::render() {
    auto renderPass = RenderPass::create()
            .withCamera(camera)
            .build();

    renderPass.draw(walls, glm::mat4(1), wallMaterial);
    renderPass.draw(floorMesh, glm::mat4(1), floorMaterial);
    renderPass.draw(ceilMesh, glm::mat4(1), ceilMaterial);

    if (debugBricks){
        renderDebugBricks(renderPass);
    }

    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x/2-100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::DragFloat("Rot", &fpsController.rotation);
    ImGui::DragFloat3("Pos", &(fpsController.position.x), 0.1f);
    ImGui::Checkbox("DebugBricks", &debugBricks);
    ImGui::Checkbox("LockRotation", &lockRotation);
    ImGui::End();
}

void Wolf3D::addFloorAndCeil(std::vector<glm::vec3>& floorVertexPositions, std::vector<glm::vec3>& ceilVertexPositions, float size) {
    float hSize = (size / 2);
    floorVertexPositions.insert(floorVertexPositions.end(), {
            //floor
            glm::vec3(-hSize,-.5,hSize), glm::vec3(hSize,-.5,hSize), glm::vec3(-hSize,-.5,-hSize),
            glm::vec3(hSize,-.5,-hSize), glm::vec3(-hSize,-.5,-hSize), glm::vec3(hSize,-.5, hSize),
    });

    ceilVertexPositions.insert(ceilVertexPositions.end(), {
            //ceil
            glm::vec3(hSize,.5,hSize), glm::vec3(-hSize,.5,hSize), glm::vec3(hSize,.5,-hSize),
            glm::vec3(hSize,.5,-hSize), glm::vec3(-hSize,.5,hSize), glm::vec3(-hSize,.5,-hSize),
    });
}

void Wolf3D::addCube(std::vector<glm::vec3>& vertexPositions, std::vector<glm::vec4>& textureCoordinates, int x, int z, int type){
    float offset = .5f;

    std::vector<glm::vec3> tempVector = {
            //front
            glm::vec3(-offset,-offset,offset), glm::vec3(offset,-offset,offset), glm::vec3(-offset,offset,offset),
            glm::vec3(offset,offset,offset), glm::vec3(-offset,offset,offset), glm::vec3(offset,-offset,offset),

            //back
            glm::vec3(offset,-offset,-offset), glm::vec3(-offset,-offset,-offset), glm::vec3(offset,offset,-offset),
            glm::vec3(-offset,offset,-offset), glm::vec3(offset,offset,-offset), glm::vec3(-offset,-offset,-offset),

            //left
            glm::vec3(offset,-offset,offset), glm::vec3(offset,-offset,-offset), glm::vec3(offset,offset,offset),
            glm::vec3(offset,offset,-offset), glm::vec3(offset,offset,offset), glm::vec3(offset,-offset,-offset),

            //right
            glm::vec3(-offset,-offset,-offset), glm::vec3(-offset,-offset,offset), glm::vec3(-offset,offset,-offset),
            glm::vec3(-offset,offset,offset), glm::vec3(-offset,offset,-offset), glm::vec3(-offset,-offset,offset),
    };

    for (int i = 0; i < tempVector.size(); ++i) {
        tempVector[i].x += x;
        tempVector[i].z += z;
        vertexPositions.insert(vertexPositions.end(),tempVector[i]);
    }

    glm::vec2 textureSize(2048,4096);
    glm::vec2 tileSize(64,64);
    glm::vec2 tileSizeWithBorder(65,65);

    int doubleTileSize = tileSizeWithBorder.x * 2;

    //I hate hardcoding this 8 but the spritesheet is very weird in order for it to be POT
    int numberOfTilesInRow = 8;

    int yOffset = tileSizeWithBorder.y * floor(type / numberOfTilesInRow);

    if(yOffset != 0) {
        type = type - (numberOfTilesInRow * floor(type / numberOfTilesInRow)) ;
    }

    type = (type * doubleTileSize);

    glm::vec2 min = vec2(type,42*tileSizeWithBorder.y - yOffset) / textureSize;
    glm::vec2 minAlt = vec2(type + tileSizeWithBorder.x,42*tileSizeWithBorder.y - yOffset) / textureSize;

    glm::vec2 max = min+tileSize / textureSize;
    glm::vec2 maxAlt = minAlt+tileSize / textureSize;

    textureCoordinates.insert(textureCoordinates.end(),{
            glm::vec4(min.x,min.y,0,0), glm::vec4(max.x,min.y,0,0), glm::vec4(min.x,max.y,0,0),
            glm::vec4(max.x,max.y,0,0), glm::vec4(min.x,max.y,0,0), glm::vec4(max.x,min.y,0,0),

            glm::vec4(min.x,min.y,0,0), glm::vec4(max.x,min.y,0,0), glm::vec4(min.x,max.y,0,0),
            glm::vec4(max.x,max.y,0,0), glm::vec4(min.x,max.y,0,0), glm::vec4(max.x,min.y,0,0),

            glm::vec4(minAlt.x,minAlt.y,0,0), glm::vec4(maxAlt.x,minAlt.y,0,0), glm::vec4(minAlt.x,maxAlt.y,0,0),
            glm::vec4(maxAlt.x,maxAlt.y,0,0), glm::vec4(minAlt.x,maxAlt.y,0,0), glm::vec4(maxAlt.x,minAlt.y,0,0),

            glm::vec4(minAlt.x,minAlt.y,0,0), glm::vec4(maxAlt.x,minAlt.y,0,0), glm::vec4(minAlt.x,maxAlt.y,0,0),
            glm::vec4(maxAlt.x,maxAlt.y,0,0), glm::vec4(minAlt.x,maxAlt.y,0,0), glm::vec4(maxAlt.x,minAlt.y,0,0),
    });
}

void Wolf3D::init() {
    wallMaterial = Shader::getUnlit()->createMaterial();
    auto texture = Texture::create().withFile("level0.png")
            .withGenerateMipmaps(false)
            .withFilterSampling(false)
            .build();
    wallMaterial->setTexture(texture);

    floorMaterial = Shader::getUnlit()->createMaterial();
    auto floorColor = sre::Color(vec4(0.44,0.44,0.44,1));
    floorMaterial->setColor(floorColor);

    ceilMaterial = Shader::getUnlit()->createMaterial();
    auto ceilColor = sre::Color(vec4(0.22,0.22,0.22,1));
    ceilMaterial->setColor(ceilColor);

    map.loadMap("level0.json");

    std::vector<glm::vec3> wallsVertexPositions;
    std::vector<glm::vec3> floorVertexPositions;
    std::vector<glm::vec3> ceilVertexPositions;
    std::vector<glm::vec4> textureCoordinates;

    for (int x=0;x<map.getWidth();x++){
        for (int z=0;z<map.getHeight();z++){
            int field = map.getTile(x ,z);
            if (field != -1){
                addCube(wallsVertexPositions, textureCoordinates, x, z, field);
            }
        }
    }

    addFloorAndCeil(floorVertexPositions, ceilVertexPositions, 20);

    fpsController.setInitialPosition(map.getStartingPosition(), map.getStartingRotation());

    walls = Mesh::create()
            .withPositions(wallsVertexPositions)
            .withUVs(textureCoordinates)
            .build();

    floorMesh = Mesh::create()
            .withPositions(floorVertexPositions)
            .build();

    ceilMesh = Mesh::create()
            .withPositions(ceilVertexPositions)
            .build();
}

void Wolf3D::renderDebugBricks(RenderPass & renderPass){
    static auto cube = Mesh::create().withCube(0.5f).build();
    static vector<shared_ptr<Material> > materials = {
            Shader::getUnlit()->createMaterial(),
            Shader::getUnlit()->createMaterial(),
            Shader::getUnlit()->createMaterial()
    };

    std::vector<vec3> positions = {
            {-1,0,-2},
            { 0,0,-3},
            { 1,0,-4}
    };
    std::vector<sre::Color> colors = {
            {1,0,0,1},
            {0,1,0,1},
            {0,0,1,1},
    };
    for (int i=0;i<positions.size();i++){
        materials[i]->setColor(colors[i]);
        renderPass.draw(cube, glm::translate(positions[i]), materials[i]);
    }
}

int main(){
    new Wolf3D();
    return 0;
}