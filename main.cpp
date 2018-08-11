#include <iostream>
#include <QString>
#include <exception>

#include "src/Logger.hpp"
#include "src/FileList.hpp"

using namespace std;

int main(int, char *[])
{
    cout << "MSPverifier" << endl;

//    QStringList suff = {".s43", ".h"};
//    QStringList ex = {"msp430fr5962", "Kradzione"};

    try
    {
        Logger::ClearFiles();
        QStringList files = FileList::GetFileList("./input/emespe.ewp");
//        FileList fl("input/", suff, ex);
//        fl.GetFileList();
//        Logger::LogError("err.what()");
//        Logger::LogWarning("err.what()");
    }
    catch(std::runtime_error err)
    {
        std::cout << "RUNTIME ERROR: " << err.what() << std::endl;
//        Logger::LogError(QString("RUNTIME ERROR: ") + QString(err.what()));
    }

    return 0;
}
