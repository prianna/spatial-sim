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


struct SeedParam
{
    SeedParam()
    : randomSeed(false), t_init(0), t_max(0), initSeed(0), numPatches(0), seedLevel(LEVEL_2)
    {}
    
    SeedParam( int S, int P, double t0, double tM, int L )
    : randomSeed(false), t_init(t0), t_max(tM), initSeed(S), numPatches(P),
    seedLevel(L)
    {}
    
    SeedParam( bool rSeed, double t0, double tM )
    : randomSeed(true), t_init(t0), t_max(tM), initSeed(0), numPatches(0),
    seedLevel(LEVEL_2)
    {}
    
    int initSeed, numPatches, seedLevel;
    bool randomSeed;
    double t_init, t_max;
};

class ModelWorld {

public:
    // Takes in the number of simulations to run in parallel, the number of the
    // current run, and a vector of parameters to send to the Gillespie sims.
    // Can either have same set of parameters for each, or different.
    ModelWorld( int numSims, int runNum, int pSet, std::vector<SeedParam> P );
    ~ModelWorld();
    
    void CallSim();
    
private:
    // Nodes
    std::vector<Patch*> nodes;
    
    // Gillespie object to simulate next state.
    std::vector<Gillespie*> SSA;
    
    std::vector<SeedParam> params;
    
    int numSSA, runNum, paramSet;
};


#endif
