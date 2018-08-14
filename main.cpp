#include <iostream>
#include <QString>
#include <stdexcept>

#include "src/Logger.hpp"
#include "src/FileList.hpp"

using namespace std;

int main(int, char *[])
{
    cout << "MSPverifier" << endl;

    try
    {
        Logger::ClearFiles();
        FileList("./input/emespe.ewp");
    }
    catch(std::runtime_error err)
    {
        std::cout << "RUNTIME ERROR: " << err.what() << std::endl;
//        Logger::LogError(QString("RUNTIME ERROR: ") + QString(err.what()));
    }

    return 0;
}
