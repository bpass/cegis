/**
 * @file BSQReader_t.cpp
 * @author Austin Hartman
 *
 * $Id: BSQReader_t.cpp,v 1.1 2005/07/06 20:52:03 ahartman Exp $
 */

#include "BSQReader.h"

int main()
{
    const std::string bsqFileName = 
        "D:/Data/agnps-output/30/littler30_nitro.bsq";
    const std::string headerFileName = 
        "D:/Data/agnps-output/30/littler30_nitro.hdr";

    BSQReader<float> reader(bsqFileName);
    BSQReader<float> reader2(bsqFileName, headerFileName);

}

