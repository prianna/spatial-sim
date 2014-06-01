//
//  main.cpp
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#include <iostream>
#include "Globals.h"
#include "ModelWorld.h"

int main(int argc, const char * argv[])
{
    ModelWorld graph( 1 );
    graph.CallSim(0, 10);
}

