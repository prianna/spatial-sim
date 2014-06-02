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
    ModelWorld( int numSims, int numRun );
    ~ModelWorld();
    
    void CallSim( int t_init, int t_max );
    void CallSim( int numSeed, int numPatches, int t_init, int t_max );
    void CallSim( std::string file );
    
private:
    // Nodes
    std::vector<Patch*> nodes;
    
    // Gillespie object to simulate next state.
    std::vector<Gillespie*> SSA;
    
    int numSSA, numRun;

};


#endif
