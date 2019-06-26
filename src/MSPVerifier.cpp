#include "MSPVerifier.hpp"

#include "Logger.hpp"
#include "FileList.hpp"
#include "segmentList.hpp"
#include "funContainer.hpp"
#include "convs.hpp"
#include <iostream>

void MSPV::verify(QString ewpFile, QString convsFile, bool quiet)
{
    std::cout << "MSPverifier" << std::endl << std::endl;

    try
    {
        Logger::ClearFiles();
        FileList fl(ewpFile);
        SegmentList sl = fl.getSegments();
        sl.store();
        FunContainer fc = sl.digForFunctions(quiet);
        Convs convs;
        convs.loadFromXmlFile(convsFile);
        convs.store();
        fc.simulate(&convs);
    }
    catch(std::runtime_error err)
    {
        std::cout << "RUNTIME ERROR: " << err.what() << std::endl;
    }
}

