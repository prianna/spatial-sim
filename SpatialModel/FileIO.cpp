//
//  FileIO.cpp
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#include "FileIO.h"

void FileIO::OpenFile(const std::string fname)
{
    /*std::size_t pos1 = fname.find("sim");
    std::size_t pos2 = fname.find("_");
    const std::string dir = fname.substr(pos1, pos2-pos1);*/

    std::ifstream check_file (fname);
    // If the file doesn't exist, create it.
    if ( !check_file.is_open() ) writeFile.open(fname);
    // Else, open file and append output.
    else writeFile.open(fname, std::ios::app );
}