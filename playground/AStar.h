//
// Created by Thomas on 15.05.18.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Graph.h"
#include "ExplorationAgenda.h"


class CAStar {
private:
    glm::vec3 fPos; //todo struct with 3dpoints and pointID
    glm::vec3 fPosTarget; //todo vielleicht nicht notwendig
    CGraph* graph;
    std::vector<int> pOutBuffer;

public:
    CAStar(CGraph* graph);
    ~CAStar();

    int FindPath(glm::vec3 fPosStart, glm::vec3 fPosTarget, const int nOutBufferSize);
    float HScore();
};



