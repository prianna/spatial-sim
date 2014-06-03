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
      NEIGHBOR5};

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
Gillespie::Gillespie( const std::vector<Patch*> States, std::string pref )
: gen(rd()), runif(0.0, 1.0), prefix(pref)
{
    double curInf = 0, curSup = 0;
    for (std::vector<Patch*>::const_iterator it = States.begin();
                                       it!= States.end(); ++it)
    {
        State cur;
        double totalRate;
        cur.adj = (*it)->GetAdjIndices();

        cur.S = (*it)->GetPopS();
        cur.I = (*it)->GetPopI();
        cur.R = (*it)->GetPopR();
        
        cur.N = cur.S+cur.I+cur.R;
        cur.beta = (*it)->GetBeta();
        cur.event[0] = (cur.beta*cur.S*cur.I);
        cur.event[1] = (GAMMA*cur.I);
        
        for (int j = 0; j < cur.adj.size(); ++j)
        {
            cur.event[j+2] = cur.N*FLOW;
        }
        
        // Calculating total rate for patch i.
        for (int i = 0; i < MAX_EVENTS; ++i)
        {
            totalRate += cur.event[i];
        }
        curInf += curSup;
        curSup += totalRate;
        
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
    
    int numInit = rNumInit(gen);
    std::vector<int> numInfected, rSeedIndex, seedIndex;
    
    for (int i = MIN_INDEX_L2; i < NUM_NODES; ++i)
    {
        rSeedIndex.push_back(i);
    }
    
    std::random_shuffle( rSeedIndex.begin(), rSeedIndex.end() );
    
    for (int i = 0; i < numInit; ++i)
    {
        numInfected.push_back(rNumInfected(gen));
        seedIndex.push_back(rSeedIndex.at(i));
    }
    
    
    for (std::vector<int>::iterator it = seedIndex.begin();
                                    it != seedIndex.end(); ++it)
    {
        curStates.at(*it).I = numInfected.at(it-seedIndex.begin());
        curStates.at(*it).S -= curStates.at(*it).I;
        curStates.at(*it).N = curStates.at(*it).S +
                              curStates.at(*it).I +
                              curStates.at(*it).R ;
    }
    
    std::string suffix = "randSeed";
    ioDevice.OpenFile(prefix+suffix);
    std::stringstream ss;
    std::string header;
    ss << "Node\tT\tS\tI\tR\tN\tR0\n";
    header = ss.str();
    ioDevice.WriteHeader(header);
    
    for (double i = t_init; i < t_max; )
    {
        i += Iterate();
        OutputStates( i );
    }
    
    ioDevice.CloseOutput();
}

void Gillespie::Simulate( int numSeed, int numPatches, int t_init, int t_max )
{
    std::vector<int> numInfected, rSeedIndex, seedIndex;
    
    for (int i = MIN_INDEX_L2; i < NUM_NODES; ++i)
    {
        rSeedIndex.push_back(i);
    }
    
    std::random_shuffle( rSeedIndex.begin(), rSeedIndex.end() );
    
    for (int i = 0; i < numPatches; ++i)
    {
        numInfected.push_back(numSeed);
        seedIndex.push_back(rSeedIndex.at(i));
    }
    
    
    for (std::vector<int>::iterator it = seedIndex.begin();
         it != seedIndex.end(); ++it)
    {
        curStates.at(*it).I = numInfected.at(it-seedIndex.begin());
        curStates.at(*it).S -= curStates.at(*it).I;
        curStates.at(*it).N = curStates.at(*it).S +
        curStates.at(*it).I +
        curStates.at(*it).R ;
    }
    
    std::string suffix = "specSeed";
    ioDevice.OpenFile(prefix+suffix);
    std::stringstream ss;
    std::string header;
    ss << "Node\tT\t\tS\tI\tR\tN\tR0\n";
    header = ss.str();
    ioDevice.WriteHeader(header);
    
    for (double i = t_init; i < t_max; )
    {
        i += Iterate();
        OutputStates( i );
    }
    
    ioDevice.CloseOutput();
}


double Gillespie::Iterate()
{
	double totalRate = ComputeTotalRate();
	double r = runif(gen);
	double dt = (-1/totalRate)*log( r ); // Time until next event.
    double p = runif(gen)*totalRate;
    
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
                            int type_i = TypeToMove( *patch );
                            int j = patch->adj.at(i-2);
                            if ( CheckMove(patchIndex, j, type_i) )
                            {
                                // Want to match movements in a 1:1 manner.
                                // Make equivalent move from patch j to patch p.
                                MakeMove(patchIndex, j, type_i);
                                int type_j = TypeToMove( curStates.at(j) );
                                MakeMove(j, patchIndex, type_j);
                            }
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
    double curInf = 0, curSup = 0;
    for (std::vector<State>::iterator cur = curStates.begin();
         cur != curStates.end(); ++cur)
    {
        cur->event[0] = ((cur->S)*(cur->I)*(cur->beta));
        cur->event[1] = GAMMA*(cur->I);
        
        for (int j = 0; j < cur->adj.size(); ++j)
        {
            int migrants = static_cast<int>((cur->N)*FLOW);
            cur->event[j+2] = migrants;
        }
        
        double totRate = 0;
        // Calculating total rate for patch i.
        for (int i = 0; i < MAX_EVENTS; ++i)
        {
            totRate += cur->event[i];
        }
        
        curInf = curSup;
        curSup += totRate;
        
        // Updating infinum and supremum for rates in patch i.
        (cur->inf) = curInf;
        (cur->sup) = curSup;
    }
    return curSup;
}

int Gillespie::TypeToMove( const State &i )
{
    double pS = static_cast<double>(i.S)/static_cast<double>(i.N);
    double pI = static_cast<double>(i.I)/static_cast<double>(i.N);
    double pR = static_cast<double>(i.R)/static_cast<double>(i.N);
    std::uniform_real_distribution<> rtypes(0, pS+pI+pR);
    double q = rtypes(gen);
    
    if (q < pS) return SUS;
    else if ( q < pS+pI ) return INF;
    else if (q < pS+pI+pR ) return REC;
    
    else
        std::cerr << "Error! No type to move." << std::endl;
    
    return -1;
}

bool Gillespie::CheckMove(int i, int j, int type)
{
    if ( (curStates.at(j).N) == 0 || (curStates.at(i).N) == 0 )
        return false;
    
    
    switch (type)
    {
        case SUS:
            if ( (curStates.at(j).S)/(curStates.at(j).N) ==
                 (curStates.at(i).S)/(curStates.at(i).N))
                return false;
            else return true;
            
        case INF:
            if ( (curStates.at(j).I)/(curStates.at(j).N) ==
                (curStates.at(i).I)/(curStates.at(i).N))
                return false;
            else return true;
            
        case REC:
            if ( (curStates.at(j).R)/(curStates.at(j).N) ==
                (curStates.at(i).R)/(curStates.at(i).N))
                return false;
            else return true;
            
        default:
            return false;
    }
    
}

void Gillespie::MakeMove(int i, int j, int type)
{
    switch (type)
    {
        case SUS:
            (curStates.at(j).S)++;
            (curStates.at(i).S)--;
            (curStates.at(j).N)++;
            (curStates.at(i).N)--;
            break;
            
        case INF:
            (curStates.at(j).I)++;
            (curStates.at(i).I)--;
            (curStates.at(j).N)++;
            (curStates.at(i).N)--;
            break;
            
        case REC:
            (curStates.at(j).R)++;
            (curStates.at(i).R)--;
            (curStates.at(j).N)++;
            (curStates.at(i).N)--;
            break;
            
        default:
            break;
    }
}



void Gillespie::OutputStates( double i )
{
    long index, S, I, R, N;
    double T = i;
    std::string R0, output;
    std::stringstream ss;
    char delim = '\t';
    
    for (std::vector<State>::iterator it = curStates.begin();
         it != curStates.end(); ++it)
    {
        
        index = it - curStates.begin();
        S = it->S;
        I = it->I;
        R = it->R;
        N = it->N;
        R0 = std::to_string(it->beta/GAMMA);
        
        ss << std::setprecision(4) << index << delim << T << delim << S << delim << I << delim
        << R << delim << N << delim << R0 << std::endl;
    }
    
    output = ss.str();
    ioDevice.WriteLine(output);
    ss.clear();
}
