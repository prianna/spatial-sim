//
//  ModelWorld.cpp
//  SpatialModel
//


#include "ModelWorld.h"

ModelWorld::ModelWorld( int numSims, int run, int pSet, std::vector<SeedParam> P )
: numSSA(numSims), runNum(run), params(P), paramSet(pSet)
{
    for (int i = 0; i < NUM_NODES; ++i)
    {
        if (i == MIN_INDEX_L0)
        { nodes.push_back( new Patch(LEVEL_0) ); }
        else if (i >= MIN_INDEX_L1 && i < MIN_INDEX_L2)
        { nodes.push_back( new Patch(LEVEL_1) ); }
        else if (i >= MIN_INDEX_L2 && i < NUM_NODES)
        { nodes.push_back( new Patch(LEVEL_2) ); }
    }
    
    for ( std::vector<Patch*>::iterator it = nodes.begin(); it != nodes.end(); ++it )
    {
        int level = (*it)->GetLevel();
        int i = 0;
        switch (level)
        {
            case LEVEL_0:
                i = static_cast<int>(it - nodes.begin());
                for (int j = MIN_INDEX_L1; j < MIN_INDEX_L2; ++j)
                {
                    (*it)->AddNeighbor(nodes.at(j), j);
                }
                break;
                
            case LEVEL_1:
                i = static_cast<int>(it - nodes.begin());
                // child = BRANCH*i + j - need to post increment.
                for (int j = BRANCH*i; j++ < BRANCH*i+BRANCH; )
                {
                    (*it)->AddNeighbor(nodes.at(j), j);
                }
                (*it)->AddNeighbor( nodes.at(LEVEL_0), LEVEL_0 );
                break;
                
            case LEVEL_2:
                i = static_cast<int>(it - nodes.begin());
                // parent = node at i-1/branch.
                (*it)->AddNeighbor(nodes.at((i-1)/BRANCH), (i-1)/BRANCH);
                
                break;
                
            default:
                break;
        }
    }
    
    for (int i = 0; i < numSims; ++i)
    {
        std::string prefix = "paramset"+std::to_string(pSet)+"run"+
                              std::to_string(run)+"sim"+std::to_string(i);
        SSA.push_back( new Gillespie(nodes, prefix) );
    }
}

ModelWorld::~ModelWorld()
{
    for ( std::vector<Patch*>::iterator it = nodes.end()-1; it >= nodes.begin(); )
    {
        delete *it;
        --it;
        nodes.pop_back();
    }
    
    for ( std::vector<Gillespie*>::iterator it = SSA.end()-1; it >= SSA.begin(); )
    {
        delete *it;
        --it;
        SSA.pop_back();
    }
}

void ModelWorld::CallSim()
{
    for ( std::vector<Gillespie*>::iterator it = SSA.begin(); it != SSA.end(); ++it)
    {
        SeedParam paramSet = params.at(it-SSA.begin());
        
        if ( paramSet.randomSeed ) (*it)->Simulate(paramSet.t_init, paramSet.t_max);
        
        else (*it)->Simulate(paramSet.initSeed, paramSet.numPatches,
                             paramSet.t_init, paramSet.t_max, paramSet.seedLevel);
    }
}