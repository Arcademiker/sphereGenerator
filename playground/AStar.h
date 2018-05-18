//
// Created by Thomas on 15.05.18.
//

#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>
#include "Graph.h"
#include "Triangle.h"
#include "ExplorationAgenda.h"


class CAStar {
private:
    CGraph* graph;
    const std::vector<CTriangle>* p3DMesh;
    std::vector<int>* pOutBuffer;
    std::vector<CTriangle::SPoint3D>* pRoute3DPoints;
    //float normEdgeLength;
    ExplorationAgenda* explorationAgenda;

    const CTriangle::SPoint3D* get3DPoint(int vertexID) const;
    //float HEuclidean(glm::vec3 fPos, glm::vec3 fPosTarget);
    float HScore(glm::vec3 fPos, glm::vec3 fPosTarget);

public:
    int expansedVertixes;
    int steps;

    CAStar(CGraph* graph,const std::vector<CTriangle>* p3DMesh);
    ~CAStar();

    float FindPath(int vertexIDStart, int vertexIDTarget, const unsigned int nOutBufferSize);

    //get route in vertexIDs
    std::vector<int>* getRoute();

    const std::vector<CTriangle::SPoint3D>* getRoute3DPoints() const;
    const std::vector<CTriangle::SPoint3D>* getExpansed3DPoints() const;
};



