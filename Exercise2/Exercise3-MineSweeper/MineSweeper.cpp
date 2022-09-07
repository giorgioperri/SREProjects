#include <iostream>
#include <cmath>
#include "MineSweeper.hpp"

MineSweeper::MineSweeper(){
    // todo implement
}

void MineSweeper::initLevel(int size){
    // based on input size we create a vector of characters that have a reasonable amount of bombs inside
    srand(time(NULL));
    for (int i = 0; i < size * size; ++i) {
        fieldMap.push_back(tuple<int,int>(0,((rand() % 9) + 1) > 3 ? 0 : 1));
    }

    for (int i = 0; i < fieldMap.size(); i += size) {
        vector<tuple<int,int>> tempvector;

        for (int j = i; j < i + size; ++j) {
            tempvector.push_back(fieldMap[j]);
        }

        fieldRows.push_back(tempvector);
    }

    // todo implement
}

void MineSweeper::print(){
    for (int i = 0; i < fieldRows.size(); ++i) {
        for (int j = 0; j < fieldRows[i].size(); ++j) {
            if(get<0>(fieldRows[i][j]) == 0) { //if element is covered
                std::cout << "X ";
            } else { // if uncovered
                std::cout << get<1>(fieldRows[i][j]) << " ";
            }
        }
        cout << endl;
    }
    // todo implement
}

FieldType MineSweeper::getField(int row, int column){
    // todo implement
    return Button;
}

bool MineSweeper::isRunning(){
    // todo implement
    return true;
}

std::string MineSweeper::take(int row, int column){
    get<0>(fieldRows[row][column]) = 1;
    // todo implement
    return "";
}

int MineSweeper::numberOfBombs(){
    // todo implement
    return 0;
}

int MineSweeper::numberOfHidden(){
    // todo implement
    return 0;
}

int MineSweeper::bombsNearby(int row, int column){
    // todo implement
    return 0;
}
