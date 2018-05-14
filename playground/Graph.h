//
// Created by Thomas Engels on 09.05.18.
//

#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

class CGraph {
private:
    std::vector<std::unordered_map<int,int>>* G;
    //std::pair<std::unordered_map<int,int>::iterator, bool> itAdjcent;
    std::vector<std::vector<unsigned int>>* m_matPointsofTraingle;
    std::unordered_map<int,int>* m_edgeList;
    size_t size;
    int edgeCounter;

public:
    CGraph(size_t size);
    ~CGraph();
    bool addEdge(int u, int v, int w);
    int getEdge(unsigned int e);
    std::unordered_map<int,int> getAdjacent(unsigned int u);
    void addTriangle(unsigned int point1,unsigned int point2,unsigned int point3, unsigned int triangleID);
    void reconstructGraph(unsigned int size);
    std::vector<unsigned int> getPointsofTriangle(size_t triangleID);
    int getSize();

    void printGraph();
};

//std::vector<std::vector<std::pair<int,int>>>* G;
