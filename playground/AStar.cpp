//
// Created by stoox on 15.05.18.
//

#include "AStar.h"

CAStar::CAStar(CGraph *graph,const std::vector<CTriangle>* p3DMesh) {
    this->graph = graph;
    this->p3DMesh = p3DMesh;
    this->pOutBuffer = new std::vector<int>;
}

CAStar::~CAStar() {
    delete this->pOutBuffer;
}


//todo: description
int CAStar::FindPath(int vertexIDStart, int vertexIDTarget, const unsigned int nOutBufferSize) {

    //exist a path variable
    bool bPath = false;

    //path length in steps (ignoring edge weights)
    int nPathLength = 0;

    //clear old pOutBuffer from last pathfinding (pOutBuffer stores the vertexIDs of the taken route)
    delete pOutBuffer;
    this->pOutBuffer = new std::vector<int>(nOutBufferSize,-1);


    //visit start vertex
    ///const int nMapSize = nMapHeight*nMapWidth;

    //translate array position on map into x and y values. nNode is a vertex on the grid with four neighbours up down left right
    ///int nNode = nStartX+nStartY*nMapWidth;
    glm::vec3 fPos = this->get3DPoint(vertexIDStart)->fPos;
    glm::vec3 fPosTarget = this->get3DPoint(vertexIDTarget)->fPos;

    //ExplorationAgenda is a Priority Queue with key value pairs. The pair with the smallest key is on top.
    //key is the float value for the f(n) score of A*.
    //the value of the Priority Queue is the VertexID.
    //why priority queue, why not multimap?
    // the std-priority_queue is faster than the std-multimap, because:
    //  The typical underlying implementation of a multimap is a balanced red/black tree.
    //  Repeated element removals from one of the extreme ends of a multimap has a good chance of skewing the tree,
    //  requiring frequent rebalancing of the entire tree. This is going to be an expensive operation.
    auto explorationAgenda = new ExplorationAgenda();
    //explore start vertex (which means calculating f(n) score for this vertex): f(n) = h(n)+g(n)
    //g(n) := dijsktra distance from start to current vertex. for the start vertex = 0
    //h(n) := manhattan distance form target to current vertex ignoring all possible walls on the way. h(n) the heuristic for this A*
    explorationAgenda->Add(HScore(fPos,fPosTarget),vertexIDStart);

    //pGScore = g(n) ; key = vertexID , value = GScore
    //why hash map, why not a vector?
    // a vector has a slightly faster access to its elements in many cases, however some maps can be very big to do pathfinding on,
    // especially in modern games. The big advantage of unordered maps (hashmaps) is, they just keep track of the visited elements.
    // If A* has to operate just in a small corner of a big map, the whole map (in case of a vector) has to be set to 0 at first
    // to initialise the g(n) score values and would consume storage that is never needed the whole time the algorithm is in use.
    auto pGScore = new std::unordered_map<int,int>();
    //iterator to work with unordered map (hash map)
    std::pair<std::unordered_map<int,int>::iterator, bool> itVisited;
    pGScore->insert(std::make_pair(vertexIDStart,0));

    //parent Tree to find the shortest path from target back to the start (key = vertex, value = parent)
    auto pParentNode = new std::unordered_map<int,int>();
    //define starting point of the parent tree (parenting itself)
    pParentNode->insert({vertexIDStart,vertexIDStart});

    //direction up = 0, left = 1, down = 2, right = 3
    std::vector<int> nDir;
    //bool bDirBorder[4];
    ///int nTarget=nTargetX+nTargetY*nMapWidth;
    //current visited vertex
    int vertexID = vertexIDStart;

    //main loop of the A* algorithm
    while(nOutBufferSize>nPathLength && !explorationAgenda->IsEmpty()) {

        //set the current vertex (nNode) to the vertex with the highest exploration priority (lowest f(n) Score)
        vertexID = explorationAgenda->VisitTop();
        nPathLength = (*pGScore)[vertexID]; //path length in number of steps

        //abort if target vertex is reached
        if (vertexIDTarget==vertexID) {

            //find the shortest way back from the target to the start
            for(int i=nPathLength; i>0; i--) {
                (*pOutBuffer)[i-1] = vertexID;
                vertexID = (*pParentNode)[vertexID];
            }
            bPath = true;
            break;
        }

        //calculate the adjacent nodes of nNode (current Node)
        for (auto u : this->graph->getAdjacent(vertexID)) {
            nDir.push_back(u.first);
        }
        ///nDir[0] = nNode-nMapWidth;  //move up in array coordinates
        ///nDir[2] = nNode+nMapWidth;  //move down in array coordinates
        ///nDir[1] = nNode-1;          //move left in array coordinates
        ///nDir[3] = nNode+1;          //move right in array coordinates

        //check if one of the previous move (nDir[]) calculations has crossed the pseudo matrix borders (which is represented by the pMap array)
        ///bDirBorder[0] = nDir[0] >= 0;
        ///bDirBorder[2] = nDir[2] < nMapSize;
        ///bDirBorder[1] = nDir[1] >= nNode/nMapWidth*nMapWidth;
        ///bDirBorder[3] = nDir[3] < nNode/nMapWidth*nMapWidth+nMapWidth;

        //try to visit each direction (6 directions in most cases. There are 12 Vertices on a tesselated icosahedron which have got 5 connecting edges)
        for(auto d: nDir) {
            //Check outer rim of the map && Check for Walls
            //todo texture coordinate read
            if (true /*static_cast<bool>(pMap[d])*/) {
                //if insertion is successful "itVisited.second" in the if-statement becomes true
                itVisited = pGScore->insert(std::make_pair(d, 0));
                //don't visit the same vertex twice
                if (itVisited.second) {
                    //set the g(n) Score (without doing a second hashmap traversal) by raising the old g(n) by 1
                    itVisited.first->second = (*pGScore)[vertexID] + 1;
                    //keep track of the last visited vertex to find the shortest way back
                    pParentNode->insert(std::make_pair(d, vertexID));
                    //visit up and write FScore (f(n)=h(n)+g(n)) as key into the priority queue
                    fPos = this->get3DPoint(vertexID)->fPos;
                    explorationAgenda->Add(HScore(fPos, fPosTarget) + itVisited.first->second, d);
                }
            }
        }
    }
    //end of the A* Algorithm

    ///print the map
    /*
    if(nMapWidth<10 && nMapHeight<10) {
        std::cout << "traversal tree ( g(n) values of a* ):" << std::endl;
        for (int i = 0; i < nMapSize; i++) {
            if (i % nMapWidth == 0) { std::cout << std::endl; }
            if (pGScore->count(i)) {
                std::cout << (*pGScore)[i] << " ";
            } else {
                std::cout << "X ";
            }
        }
        std::cout << std::endl << std::endl;
    }
    */

    delete explorationAgenda;
    delete pParentNode;
    delete pGScore;

    //return -1 when there is no way or the nOutBufferSize is too short
    if(!bPath) {
        if(nOutBufferSize<=nPathLength) {
            std::cout << std::endl << "Pathfinding was aborted, because there is no way to the target within the distance of "
                      << nOutBufferSize << " (nOutBufferSize)" << std::endl;
        }
        nPathLength = -1;
    }

    return nPathLength;
}

const CTriangle::SPoint3D* CAStar::get3DPoint(int vertexID) const {
    //todo TPID explain
    std::pair<int,int> TPID = this->graph->get3DPointIDofVertexID(vertexID);
    return this->p3DMesh->at(TPID.first).GetPoints(TPID.second);
}

//todo: quadrat of hscore ok? scalieren!
float CAStar::HScore(glm::vec3 fPos, glm::vec3 fPosTarget) {
    return glm::sqrt((fPos.x-fPosTarget.x)*(fPos.x-fPosTarget.x)
           +(fPos.y-fPosTarget.y)*(fPos.y-fPosTarget.y)
           +(fPos.z-fPosTarget.z)*(fPos.z-fPosTarget.z));
}

std::vector<int>* CAStar::getRoute() {
    return this->pOutBuffer;
}






