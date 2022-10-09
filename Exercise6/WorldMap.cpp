//
// Created by Morten Nobel-Jørgensen on 22/09/2017.
//

#include "WorldMap.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>

using namespace std;

void WorldMap::loadMap(std::string filename) {

    using namespace rapidjson;
    ifstream fis(filename);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    const Value& tileMap = d["tileMap"];

    for (int i = 0; i < tileMap.GetArray().Size(); ++i) {
        std::vector<int> vals;
        for (int j = 0; j < tileMap.GetArray()[i].Size(); ++j) {
            vals.push_back(tileMap.GetArray()[i].GetArray()[j].GetInt());
        }
        values.push_back(vals);
    }

    const Value& spawn = d["spawn"];

    startingPosition.x = spawn.GetObject()["x"].GetFloat();
    startingPosition.y = spawn.GetObject()["y"].GetFloat();
    startingRotation = spawn.GetObject()["angle"].GetFloat();

    const Value& floorCol = d["floorColor"];
    const Value& ceilCol = d["ceilColor"];

    std::cout << floorCol[0].GetFloat() << std::endl;

    floorColor = glm::vec4(floorCol[0].GetFloat(), floorCol[1].GetFloat(),
                           floorCol[2].GetFloat(), floorCol[3].GetFloat());

    ceilColor = glm::vec4(ceilCol[0].GetFloat(), ceilCol[1].GetFloat(),
                          ceilCol[2].GetFloat(), ceilCol[3].GetFloat());

}

int WorldMap::getTile(int x, int y) {
    return values.at(y).at(x);
}

int WorldMap::getWidth() {
    return values[0].size();
}

int WorldMap::getHeight() {
    return values.size();
}

glm::vec2 WorldMap::getStartingPosition() {
    return startingPosition;
}

float WorldMap::getStartingRotation() {
    return startingRotation;
}

glm::vec4 WorldMap::getFloorColor() {
    return floorColor;
}

glm::vec4 WorldMap::getCeilColor() {
    return ceilColor;
}
