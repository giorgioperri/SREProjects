#include "MineSweeper.hpp"

#include <sstream>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

MineSweeper::MineSweeper() {
    srand(time(nullptr));
}

void MineSweeper::initLevel(int size) {
    _size = size;
    _nBombs = _size; // totally accurate bomb simulator

    // init field
    _field = vector<vector<FieldType>>(_size);
    for (int i = 0; i < _size; ++i)
        _field[i] = vector<FieldType>(_size, FieldType::Button);

    // init bombs
    _bombs = vector<vector<bool>>(_size);
    for (int i = 0; i < _size; ++i)
        _bombs[i] = vector<bool>(_size, 0);

    // spawn bombs
    for (int i = 0; i < _nBombs; ++i) {
        int x = 0, y = 0;
        do {
            x = rand() % size;
            y = rand() % size;
        } while (_bombs[x][y]);
        _bombs[x][y] = true;
    }

    _running = true;
}

void MineSweeper::print(){
    for(int i = 0; i < _size; ++i) {
        cout << endl;
        for(int j = 0; j < _size; ++j)
            cout << renderMap[getField(i, j)];

#ifndef NDEBUG
        cout << '\t';
        cout << '(';
        for(int j = 0; j < _size; ++j)
            cout << _bombs[i][j] ? 'X' : 'O';
        cout << ')';
#endif

    }
}

FieldType MineSweeper::getField(int row, int column){
    return _field[row][column];
}

bool MineSweeper::isRunning(){
    return _running;
}

std::string MineSweeper::take(int row, int column){
    stringstream msg;

    // check row range
    if (row < 0 || row >= _size) {
        msg << "Row must be between 0 and " << _size << endl;
        return msg.str();
    }

    // check col range
    if (column < 0 || column >= _size) {
        msg << "Column must be between 0 and " << _size << endl;
        return msg.str();
    }

    // check game over condition (bomb hit)
    if (_bombs[row][column])
    {
        _running = false;
        _field[row][column] = FieldType::BombExploded;
        msg << "Got a Bomb! Game Over" << endl;
        return msg.str();
    }

    // check already pressed
    if (_field[row][column] != FieldType::Button)
    {
        msg << "Already seen this..." << endl;
        return msg.str();
    }

    // flood fill
    int nearby = _iTake(row, column);
//    _field[row][column] = (FieldType)(nearby + 1);
    if (nearby > 0)
        msg << "That was close! " << nearby << " bombs nearby." << endl;
    else
        msg << "No bombs nearby, expanding." << endl;

    // check win condition (only bombs are hidden)
    if (numberOfHidden() == numberOfBombs())
    {
        _running = false;
        msg << "Only bombs remain! You Win!" << endl;
        return msg.str();
    }

    return msg.str();
}

int MineSweeper::numberOfBombs() {
    return _nBombs;
}

int MineSweeper::numberOfHidden(){
    int ret = 0;

    for (auto& row : _field)
        for(auto& cell : row)
            if(cell == FieldType::Button)
                ++ret;
    return 0;
}

int MineSweeper::bombsNearby(int row, int column){
    int ret = 0;

    for(int i = max(0, row - 1); i < min(_size, row + 2); ++i)
        for(int j = max(0, column - 1); j < min(_size, column + 2); ++j)
            if (_bombs[i][j])
                ++ret;

    return ret;
}

int MineSweeper::_iTake(int row, int column)
{
    int nearby = bombsNearby(row, column);

    _field[row][column] = (FieldType)(nearby + 1);

    if (nearby > 0)
        return nearby;

    int xMax = min(_size, row + 2);
    int yMax = min(_size, column + 2);
    for(int i = max(0, row - 1); i < xMax; ++i)
        for(int j = max(0, column - 1); j < yMax; ++j)
            if(_field[i][j] == FieldType::Button)
                _iTake(i, j);

    return 0;
}
