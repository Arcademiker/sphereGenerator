//
// Created by Thomas on 15.05.18.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Graph.h"
#include "Triangle.h"
#include "ExplorationAgenda.h"


class CAStar {
private:
    //glm::vec3 fPos; //todo struct with 3dpoints and pointID
    //glm::vec3 fPosTarget; //todo vielleicht nicht notwendig

    CGraph* graph;
    const std::vector<CTriangle>* p3DMesh;
    std::vector<int>* pOutBuffer;
    std::vector<CTriangle::SPoint3D>* pRoute3DPoints;

    const CTriangle::SPoint3D* get3DPoint(int vertexID) const;
    float HScore(glm::vec3 fPos, glm::vec3 fPosTarget);

public:
    CAStar(CGraph* graph,const std::vector<CTriangle>* p3DMesh);
    ~CAStar();

    int FindPath(int vertexIDStart, int vertexIDTarget, const unsigned int nOutBufferSize);

    //get route in vertexIDs
    std::vector<int>* getRoute();

    const std::vector<CTriangle::SPoint3D>* getRoute3DPoints() const;
};



