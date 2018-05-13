//
// Created by Thomas Engels on 09.05.18.
//

#pragma once

#include <vector>
#include <iostream>

class CGraph {
private:
    std::vector<std::vector<std::pair<int,int>>>* G;
    std::vector<std::vector<int>>* m_matPointsofTraingle;
    unsigned int size;

public:
    explicit CGraph(unsigned int size);
    ~CGraph();
    void addEdge(int u, int v, int w);
    std::vector<std::pair<int,int>> getEdge(unsigned int u);
    void addTriangle(unsigned int point1,unsigned int point2,unsigned int point3, unsigned int triangle);
    void reconstructGraph(int size);
    std::vector<int> getPointsofTriangle(size_t triangleID);
    int getSize();

    void printGraph();
};

