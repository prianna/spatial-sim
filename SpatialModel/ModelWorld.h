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
    template<typename T>
    SeedParam( int P, T S, double t0, double tM )
    : numPatches(P), t_init(t0), t_max(tM), randomSeed(false)
    {

    }
    
    SeedParam( int P, bool rSeed, double t0, double tM )
    : numPatches(P), randomSeed(true), t_init(t0), t_max(tM)
    {
        
        std::random_device rd;
        std::mt19937 rnum(rd());
        std::uniform_int_distribution<> rdseed(1, INIT_S_2);
        initialSeed = rdseed(rnum);
    }
    
    int numPatches;
    int initialSeed;
    bool randomSeed;
    double t_init, t_max;
};

class ModelWorld {

public:
    // Takes in the number of simulations to run in parallel, the number of the
    // current run, and a vector of parameters to send to the Gillespie sims.
    // Can either have same set of parameters for each, or different.
    ModelWorld( int numSims, int runNum );
    ~ModelWorld();
    
    void CallSim( int t_init, int t_max );
    void CallSim( int numSeed, int numPatches, int t_init, int t_max );
    void CallSim( std::string file );
    
private:
    // Nodes
    std::vector<Patch*> nodes;
    
    // Gillespie object to simulate next state.
    std::vector<Gillespie*> SSA;
    
    std::vector<SeedParam> params;
    
    int numSSA, runNum;

};


#endif
