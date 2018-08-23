#include <iostream>
#include <QString>
#include <stdexcept>

#include "src/Logger.hpp"
#include "src/FileList.hpp"
#include "src/segmentList.hpp"
#include "src/funContainer.hpp"
#include "src/convs.hpp"

using namespace std;

int main(int, char *[])
{
    cout << "MSPverifier" << endl;

    try
    {
        Logger::ClearFiles();
        FileList fl("./input/emespe.ewp");
        SegmentList sl = fl.getSegments();
        sl.store();
        FunContainer fc = sl.digForFunctions();
        Convs convs;
        convs.loadFromXmlFile("convs.xml");
        convs.store();
        fc.simulate(&convs);
    }
    catch(std::runtime_error err)
    {
        std::cout << "RUNTIME ERROR: " << err.what() << std::endl;
//        Logger::LogError(QString("RUNTIME ERROR: ") + QString(err.what()));
    }

    return 0;
}
