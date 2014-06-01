//
//  ModelWorld.h
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#ifndef SpatialModel_ModelWorld_h
#define SpatialModel_ModelWorld_h
#include <vector>
#include <random>
#include <chrono>
#include <queue>
#include <algorithm>
#include "Gillespie.h"
#include "Patch.h"
#include "Globals.h"
class ModelWorld {

public:
    ModelWorld();
    ~ModelWorld();
    
    double GetDij( int i, int j ){ return matrixD[i][j]; }
    
private:
    // Adjacency matrix (edges) between nodes.
    double matrixA[NUM_NODES][NUM_NODES];
    
    // Dispersal matrix.
    double matrixD[NUM_NODES][NUM_NODES];
    
    // Nodes
    std::vector<Patch*> nodes;
    
    // Gillespie object to simulate next state.
    Gillespie *SSA;

};


#endif
