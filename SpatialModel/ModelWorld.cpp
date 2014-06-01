//
//  ModelWorld.cpp
//  SpatialModel
//


#include "ModelWorld.h"

ModelWorld::ModelWorld()
{
    for (int i = 0; i < NUM_NODES; ++i)
    {
        if (i == INDEX_L0){ nodes.push_back( new Patch(LEVEL_0)); }
        if (i >= INDEX_L1 && i < INDEX_L2) { nodes.push_back( new Patch(LEVEL_1)); }
        if (i >= INDEX_L2 && i < NUM_NODES) { nodes.push_back( new Patch(LEVEL_2)); }
    }
    
    for ( std::vector<Patch*>::iterator it = nodes.begin(); it != nodes.end(); ++it )
    {
        int level = (*it)->GetLevel();
        int i = 0;
        switch (level)
        {
            case LEVEL_0:
                i = static_cast<int>(it - nodes.begin());
                for (int j = LEVEL_1; j < LEVEL_2; ++j)
                {
                    (*it)->AddNeighbor(nodes.at(j), j);
                }
                break;
                
            case LEVEL_1:
                i = static_cast<int>(it - nodes.begin());
                // child = BRANCH*i + j;
                for (int j = LEVEL_2; j < BRANCH*i+BRANCH; ++j)
                {
                    (*it)->AddNeighbor(nodes.at(j), j);
                }
                (*it)->AddNeighbor( nodes.at(LEVEL_0), LEVEL_0 );
                break;
                
            case LEVEL_2:
                i = static_cast<int>(it - nodes.begin());
                // child = BRANCH*i + j;
                for (int j = LEVEL_2; j < BRANCH*i+BRANCH; ++j)
                {
                    (*it)->AddNeighbor(nodes.at(j), j);
                }
                break;
                
            default:
                
                break;
        }
    }
}