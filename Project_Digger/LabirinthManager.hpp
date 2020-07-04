#pragma once

#include "Objects/Dirt.hpp"
#include "Objects/Digger.hpp"

const int LABIRINTH_HEIGHT = 10;
const int LABIRINTH_WIDTH = 15;
const int GRAPH_WIDTH = LABIRINTH_WIDTH;
const int GRAPH_HEIGHT= LABIRINTH_HEIGHT;

enum LabirinthMode {
    LAB_OFF = -1,
    LAB_START,
    LAB_RUNNING,
    LAB_WON,
    LAB_LOST
};

class LabirinthManager {

public:

    LabirinthManager();
    LabirinthManager(const LabirinthManager&) = delete;
    LabirinthManager& operator=(const LabirinthManager&) = delete;
    ~LabirinthManager();

    LabirinthMode update();
    void draw();
    void drawGUI();
    void createLabirinth();
    void createFinish();
    bool canMove(int x, int y, const Direction& dir) const;

private:

    void clear();
    void randomizedKruskal();

private:

    Dirt* labirinth[LABIRINTH_HEIGHT][LABIRINTH_WIDTH];
    short graph[GRAPH_HEIGHT][GRAPH_WIDTH];
    Digger* player;
    int finishX;
    int finishY;
    size_t startTime;
    bool playerCanMove;

};

