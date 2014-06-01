//
//  FileIO.h
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#ifndef __SpatialModel__FileIO__
#define __SpatialModel__FileIO__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

class FileIO
{
public:
    void OpenFile( std::string fname );
    void WriteLine( std::string line ){ writeFile << line << '\n'; }
    void WriteHeader( std::string header ){ writeFile << header << '\n'; }
    void CloseOutput(){ writeFile.close(); }
    void CloseInput(){ readFile.close(); }
    
    std::string Read( std::string fileName );
    
private:
    std::ifstream readFile;
    std::ofstream writeFile;
    
};

#endif /* defined(__SpatialModel__FileIO__) */
