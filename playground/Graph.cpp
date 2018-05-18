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
 *
 * elimante all unused data structures (if not useable for colorpicking)
 * eliminate - edges
 * one direction circleing without if statements
 * struct solution!
 * /-Y-/ plot above tesselation
 * renaming of nIterator m_...
 *
 * edge weightlist or refill the edge id or if edge complexer than just weights floats its ok
 * edges need xyz
 * replace old datastructure?

 */


CGraph::CGraph(size_t size) {
    this->size = size;
    this->G = new std::vector<std::unordered_map<int,int>>(size,std::unordered_map<int,int>(6));
    //this->G3D = new std::vector<CTriangle::SPoint3D*>(size);
    this->G3DTable = new std::vector<std::pair<int,int>>(size);
    //this->G = std::vector<std::unordered_map<int,int>>();
    this->m_matPointsofTraingle = new std::vector<std::vector<int>>;
    this->m_edgeList = new std::unordered_map<int,float>;
    this->edgeCounter = 1;
    this->triangleCounter = 0;
}

CGraph::~CGraph() {
    delete this->G;
    //delete this->G3D;
    delete this->G3DTable;
    delete this->m_matPointsofTraingle;
    delete this->m_edgeList;
}


///water low weight but land to water high cost
bool CGraph::addEdge(int u, int v, float w) {
    //e = edge id
    //std::cout << "insert " << u << "--" << v << std::endl;
    //int test = this->G->at(v).count(u);
    //itAdjcent = G->at(u).insert(std::make_pair(v, 0));
    if(this->G->at(u).count(v)==0) // test if edge to u does not exist in v. average case const time
    {
        //todo: more intelligent insertion check
        this->G->at(u).insert({v,edgeCounter}); // = edgeCounter;
        this->G->at(v).insert({u,-edgeCounter});

        //fill edge weights in edge list
        (*this->m_edgeList)[edgeCounter] = w; //pair for weights w
        (*this->m_edgeList)[-edgeCounter] = w;
        edgeCounter++;
        //todo: add edge w=weight list
        //this->printGraph();
        return true;
    }
    return false;
}



void CGraph::addTriangle(int point1, int point2, int point3) {
    //todo split pair<ID,SP3D> into id -> add edge and add SP3D to vertexSP3D table
    addEdge(point1, point2,1.0f);

    addEdge(point2, point3,1.0f);

    addEdge(point3, point1,1.0f);
    std::vector<int> triangle {point1,point2,point3}; //tood 3dpoints
    this->m_matPointsofTraingle->push_back(triangle);

    this->G3DTable->at(point1) = std::make_pair(this->triangleCounter,0);
    this->G3DTable->at(point2) = std::make_pair(this->triangleCounter,1);
    this->G3DTable->at(point3) = std::make_pair(this->triangleCounter,2);

    this->triangleCounter++;
}

std::pair<int,int> CGraph::get3DPointIDofVertexID(int VertexID) {
    return this->G3DTable->at(VertexID);
}

void CGraph::reconstructGraph(size_t size) {
    delete this->G;
    delete this->m_matPointsofTraingle;
    delete this->m_edgeList;
    delete this->G3DTable;
    //delete this->G3D;
    this->size = size;
    this->G = new std::vector<std::unordered_map<int,int>>(size,std::unordered_map<int,int>(6));
    this->G3DTable = new std::vector<std::pair<int,int>>(size);
    //this->G3D = new std::vector<CTriangle::SPoint3D*>(size);
    this->m_matPointsofTraingle = new std::vector<std::vector<int>>;
    this->m_edgeList = new std::unordered_map<int,float>;
    this->edgeCounter = 1;
    //this->edgeCounter = 0; //todo????
    this->triangleCounter = 0;
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

//todo get trianglelist without ids
std::vector<int> CGraph::getPointsofTriangle(int triangleID) {
    //todo this performs a deep copy for the case this datastructure gets more complex
    std::vector<int> triangle((*m_matPointsofTraingle)[triangleID]);
    return triangle;
}

size_t CGraph::getSize() {
    return this->size;
}


float CGraph::getEdgeWeight(int e) {
    //todo return edge pointing on vertex withing specific triangle
    return this->m_edgeList->at(e);
}

std::unordered_map<int,int> CGraph::getAdjacent(int u) {
    return this->G->at(u);
}

int CGraph::getNTriangles() {
    return this->triangleCounter;
}

void CGraph::setEdgeWeights(int VertexUID, int VertexVID, float w) {
    this->m_edgeList->at(this->G->at(VertexUID).at(VertexVID)) = w;
    this->m_edgeList->at(this->G->at(VertexVID).at(VertexUID)) = w;
}

int CGraph::getEdgeCounter() {
    return this->edgeCounter;
}









