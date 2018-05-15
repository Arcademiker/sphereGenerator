//
// Created by stoox on 15.05.18.
//

#include "AStar.h"

CAStar::CAStar(CGraph *graph) {
    this->graph = graph;
    this->fPos =  glm::vec3(0.0f, 0.0f, 0.0f);
    this->fPosTarget = glm::vec3(0.0f, 0.0f, 0.0f);
}

int CAStar::FindPath(glm::vec3 fPosStart, glm::vec3 fPosTarget, const int nOutBufferSize) {
    /*
    //exist a path variable
    bool bPath = false;

    int nPathLength = 0;
    //visit start node
    const int nMapSize = nMapHeight*nMapWidth;

    //translate array position on map into x and y values. nNode is a vertex on the grid with four neighbours up down left right
    int nNode = nStartX+nStartY*nMapWidth;

    //ExplorationAgenda is a Priority Queue with key value pairs. The pair with the smallest key is on top.
    //key is the value for the f(n) score of A*.
    //why priority queue, why not multimap?
    // the std-priority_queue is faster than the std-multimap, because:
    //  The typical underlying implementation of a multimap is a balanced red/black tree.
    //  Repeated element removals from one of the extreme ends of a multimap has a good chance of skewing the tree,
    //  requiring frequent rebalancing of the entire tree. This is going to be an expensive operation.
    auto explorationAgenda = new ExplorationAgenda();
    //explore start node (which means calculating f(n) score for this node): f(n) = h(n)+g(n)
    //g(n) := dijsktra distance from start to current node. for the start node = 0
    //h(n) := manhattan distance form target to current node ignoring all possible walls on the way. h(n) the heuristic for this A*
    explorationAgenda->Add(HScore(nNode,nMapWidth,nTargetX,nTargetY),nNode);

    //pGScore = g(n)
    //why hash map, why not a vector?
    // a vector has a slightly faster access to its elements in many cases, however maps to do pathfinding on can be very big,
    // especially in modern games. The big advantage of unordered maps (hashmaps) is, they just keep track of the visited elements
    // If A* has to operate just in a small corner of a big map the whole map vector for the g(n) score has to be set to 0 at first
    // and would consume storage the whole time the algorithm is in use.
    auto pGScore = new std::unordered_map<int,int>();
    //iterator to work with unordered map (hash map)
    std::pair<std::unordered_map<int,int>::iterator, bool> itVisited;
    pGScore->insert(std::make_pair(nNode,0));

    //parent Tree to find the shortest path from target back to the start (key = node, value = parent)
    auto pParentNode = new std::unordered_map<int,int>();
    //define starting point of the parent tree
    pParentNode->insert({nNode,nNode});

    //direction up = 0, left = 1, down = 2, right = 3
    int nDir[4];
    bool bDirBorder[4];
    int nTarget=nTargetX+nTargetY*nMapWidth;

    //main loop of the A* algorithm
    while(nOutBufferSize>nPathLength && !explorationAgenda->IsEmpty()) {

        //set the current node (nNode) to the node with the highest exploration priority (lowest f(n) Score)
        nNode = explorationAgenda->VisitTop();
        nPathLength = (*pGScore)[nNode];

        //abort if target node is reached
        if (nTarget==nNode) {

            //find the shortest way back from the target to the start
            for(int i=nPathLength; i>0; i--) {
                pOutBuffer[i-1] = nNode;
                nNode = (*pParentNode)[nNode];
            }
            bPath = true;
            break;
        }

        //calculate the adjacent nodes of nNode (current Node)
        nDir[0] = nNode-nMapWidth;  //move up in array coordinates
        nDir[2] = nNode+nMapWidth;  //move down in array coordinates
        nDir[1] = nNode-1;          //move left in array coordinates
        nDir[3] = nNode+1;          //move right in array coordinates

        //check if one of the previous move (nDir[]) calculations has crossed the pseudo matrix borders (which is represented by the pMap array)
        bDirBorder[0] = nDir[0] >= 0;
        bDirBorder[2] = nDir[2] < nMapSize;
        bDirBorder[1] = nDir[1] >= nNode/nMapWidth*nMapWidth;
        bDirBorder[3] = nDir[3] < nNode/nMapWidth*nMapWidth+nMapWidth;

        //try to visit each direction (up d=0, left d=1, down d=2 and right d=3)
        for(int d=0; d<4; d++) {
            //Check outer rim of the map && Check for Walls
            if (bDirBorder[d] && static_cast<bool>(pMap[nDir[d]])) {
                //if insertion is successful "itVisited.second" in the if-statement becomes true
                itVisited = pGScore->insert(std::make_pair(nDir[d], 0));
                //don't visit the same node twice
                if (itVisited.second) {
                    //set the g(n) Score (without doing a second hashmap traversal) by raising the old g(n) by 1
                    itVisited.first->second = (*pGScore)[nNode] + 1;
                    //keep track of the last visited node to find the shortest way back
                    pParentNode->insert(std::make_pair(nDir[d], nNode));
                    //visit up and write FScore (f(n)=h(n)+g(n)) as key into the priority queue
                    explorationAgenda->Add(HScore(nDir[d], nMapWidth, nTargetX, nTargetY) + itVisited.first->second, nDir[d]);
                }
            }
        }
    }
    //end of the A* Algorithm

    //print the map
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
     */
}



