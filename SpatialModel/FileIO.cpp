//
//  FileIO.cpp
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#include "FileIO.h"

void FileIO::OpenFile(std::string fname)
{
    std::ifstream check_file (fname);
    
    if ( !check_file.is_open() ) // If the file doesn't exist, create it.
    {                              // Else, open file and append output.
        writeFile.open( fname );
    }
    
}
