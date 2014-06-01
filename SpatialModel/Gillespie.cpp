//
//  Gillespie.cpp
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#include "Gillespie.h"

enum {INFECTION, RECOVERY, NEIGHBOR1,
      NEIGHBOR2, NEIGHBOR3, NEIGHBOR4,
      NEIGHBOR5} EVENTS;

//! \brief A generic binary search using C++ templates and iterators
//! \param begin Iterator pointing to the first element
//! \param end Iterator pointing to one past the last element
//! \param key The value to be searched for
//! \return Iterator pointing to the location of the value in the given
//! vector, or one past the end if the value was not found.
template<typename Iterator, typename T>
Iterator binSearch(Iterator& begin, Iterator& end, const T& key)
{
    // Keep halving the search space until we reach the end of the vector
    Iterator NotFound = end;
    
    while(begin < end)
    {
        // Find the median value between the iterators
        Iterator Middle = begin + (std::distance(begin, end) / 2);
        
        // Re-adjust the iterators based on the median value
        if(*Middle == key)
        {
            return Middle;
        }
        else if(*Middle > key)
        {
            end = Middle;
        }
        else
        {
            begin = Middle + 1;
        }
    }
    
    return NotFound;
}

//! \brief Constructor for class Gillespie.
//! \param States Vector of patch variables to be "loaded" as states.
Gillespie::Gillespie( const std::vector<Patch*> States, int run )
: gen(rd()), runif(0.0, 1.0), prefix("run"+std::to_string(run)+"_")
{
    double curInf = 0, curSup = 0;
    for (std::vector<Patch*>::const_iterator it = States.begin();
                                       it!= States.end(); ++it)
    {
        State cur;
        cur.adj = (*it)->GetAdjIndices();

        cur.S = (*it)->GetPopS();
        cur.I = (*it)->GetPopI();
        cur.R = (*it)->GetPopR();
        
        cur.N = cur.S+cur.I+cur.R;
        
        cur.event[0] = (((*it)->GetBeta())*cur.S*cur.I);
        cur.event[1] = (GAMMA*cur.I);
        for (int j = 0; j < cur.adj.size(); ++j)
        {
            cur.event[j+2] = cur.N*FLOW;
        }
        
        // Calculating total rate for patch i.
        for (int i = 0; i < MAX_EVENTS; ++i)
        {
            cur.totalRate += cur.event[i];
        }
        
        curInf += curSup;
        curSup += cur.totalRate;
        
        // Updating infinum and supremum for rates in patch i.
        cur.inf = curInf;
        cur.sup = curSup;
        
        curStates.push_back(cur);
    }
}

void Gillespie::Simulate( int t_init, int t_max )
{
    std::uniform_int_distribution<> rNumInit(1, NUM_NODES-MIN_INDEX_L2);
    std::uniform_int_distribution<> rNumInfected(1, INIT_S_2);
    std::uniform_int_distribution<> rSeedIndex(MIN_INDEX_L2, NUM_NODES-1);
    
    int numInit = rNumInit(gen);
    std::vector<int> numInfected, seedIndex;
    
    for (int i = 0; i < numInit; ++i)
    {
        seedIndex.push_back(rSeedIndex(gen));
        numInfected.push_back(rNumInfected(gen));
    }
    
    for (std::vector<int>::iterator it = seedIndex.begin();
                                    it != seedIndex.end(); ++it)
    {
        curStates.at(*it).I = numInfected.at(it-seedIndex.begin());
        curStates.at(*it).S -= curStates.at(*it).I;
    }
    
    std::string suffix = "randSeed";
    ioDevice.OpenFile(suffix+prefix);
    std::string header = "Node\tT\tS\tI\tR\n";
    ioDevice.WriteHeader(header);
    
    for (double i = t_init; i < t_max; )
    {
        i += Iterate();
        OutputStates( i );
    }
    
    ioDevice.CloseFile(suffix+prefix);
}

double Gillespie::Iterate()
{
	double totalRate = ComputeTotalRate();
	double r = (((double) rand() / (RAND_MAX)));
	double dt = (-1/totalRate)*log( r ); // Time until next event.
    double p = (((double) rand() / (RAND_MAX)))*totalRate;
    
    // Binary search for proper event.
    std::vector<State>::iterator itB = curStates.begin(), itE = curStates.end();
    std::vector<State>::iterator found = binSearch( itB, itE, p );
    
    if ( found == curStates.end() )
    {
        std::cerr << "Search returned no events." << std::endl;
    }
    else
    {
        int patchIndex = static_cast<int>(found-itB);
        State *patch = &(*found);
        double curInf = patch->inf, curSup = patch->inf;
        for( int i = 0; i < MAX_EVENTS; ++i )
        {
            curSup += patch->event[i];
            if ( curInf < p && p <= curSup )
            {
                switch (i)
                {
                    case INFECTION:
                        (patch->S)--;
                        (patch->I)++;
                        i = MAX_EVENTS;
                        break;
                        
                    case RECOVERY:
                        (patch->I)--;
                        (patch->R)++;
                        i = MAX_EVENTS;
                        break;
                    
                    // Following cases all represent movement.
                    // This is so ugly. Maybe functionalize this.
                    case NEIGHBOR1:
                    case NEIGHBOR2:
                    case NEIGHBOR3:
                    case NEIGHBOR4:
                    case NEIGHBOR5:
                        if (patch->event[i] == 0 )
                        {
                            i = MAX_EVENTS;
                            break;
                        }
                        else
                        {
                            // Decide which type of individual to move.
                            int type = TypeToMove( *patch );
                            int j = patch->adj.at(i-2);
                            // Move individual from patch p to patch j.
                            MakeMove(patchIndex, j, type);
                            
                            // Want to match movements in a 1:1 manner.
                            // Decide which type of individual to move from j.
                            type = TypeToMove( curStates.at(j) );
                            // Make equivalent move from patch j to patch p.
                            MakeMove(j, patchIndex, type);

                            i = MAX_EVENTS;
                        }
                        break;
                        
                    default:
                        break;
                }
            }
            else
                curInf += patch->event[i];
        }
    }
    
	return dt;
}

double Gillespie::ComputeTotalRate()
{
    double totRate = 0;
    
    for (std::vector<State>::iterator it = curStates.begin();
                                    it != curStates.end(); ++it)
    {
        for (int i = 0; i < MAX_EVENTS; ++i)
        {
            totRate += it->event[i];
        }
    }
    
    return totRate;
}

int Gillespie::TypeToMove(const State &i )
{
    double pS = i.S/i.N, pI = i.I/i.N, pR = i.R/i.N;
    double q = runif(gen);
    
    if (q < pS) { return SUS; }
    if ( q < pS+pI ) { return INF; }
    if ( q < pS+pI+pR ){ return REC; }
    
    // error
    return -1;
}

void Gillespie::MakeMove(int i, int j, int type)
{
    switch (type)
    {
        case SUS:
            (curStates.at(j).S)++;
            (curStates.at(i).S)--;
            break;
            
        case INF:
            (curStates.at(j).I)++;
            (curStates.at(i).I)--;
            break;
            
        case REC:
            (curStates.at(j).R)++;
            (curStates.at(i).R)--;

            break;
            
        default:
            break;
    }
}

void Gillespie::OutputStates( double i )
{
    char index, S, I, R, T = i, delim = '\t';
    
    for (std::vector<State>::iterator it = curStates.begin();
         it != curStates.end(); ++it)
    {
        index = static_cast<char>(it - curStates.begin());
        S = static_cast<char>(it->S);
        I = static_cast<char>(it->I);
        R = static_cast<char>(it->R);
        std::string line({index, delim, T, delim, S, delim, I, delim, R});
        ioDevice.WriteLine(line);
    }
}
