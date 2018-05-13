//
// Created by Thomas on 09.05.18.
//

#include "Graph.h"

/* todo:
 * vertex reference to 3D point and vice versa: solution same numbering
 * weights pair vector
 * order doesnt matter
 * 3D heuristik
 * DOD vs OOD

 */


CGraph::CGraph(const unsigned int size) {
    this->size = size;
    this->G = new std::vector<std::vector<std::pair<int,int>>>(size);
    this->m_matPointsofTraingle = new std::vector<std::vector<int>>;
}

CGraph::~CGraph() {
    delete this->G;
}



void CGraph::addEdge(int u, int v, int w) {
    this->G->at(u).push_back({v,w});
    //this->G->at(v).push_back({u,w});
}


void CGraph::addTriangle(unsigned int point1, unsigned int point2, unsigned int point3, unsigned int triangle) {
    addEdge(point1, point2, 1);
    addEdge(point2, point3, 1);
    addEdge(point3, point1, 1);
    this->m_matPointsofTraingle[triangle].push_back({point1,point2,point3});
}

void CGraph::reconstructGraph(int size) {
    delete this->G;
    this->G = new std::vector<std::vector<std::pair<int,int>>>(size);
}

void CGraph::printGraph() {
    for (int v = 0; v < this->G->size(); ++v) {
        std::cout << "\n Adjacency list of vertex " << v << "\n head ";
        for (auto x : this->G->at(v)) {
            std::cout << "-> [" << x.first << ", " << x.second << "] ";
        }
        std::printf("\n");
    }
}

std::vector<int> CGraph::getPointsofTriangle(size_t triangleID) {
    //todo this performs a deep copy for the case this datastructure gets more complex
    std::vector<int> triangle((*m_matPointsofTraingle)[triangleID]);
    return triangle;
}

int CGraph::getSize() {
    return this->size;
}





