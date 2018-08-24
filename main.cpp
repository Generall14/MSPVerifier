#include <iostream>
#include <QString>
#include <stdexcept>

#include "src/Logger.hpp"
#include "src/FileList.hpp"
#include "src/segmentList.hpp"
#include "src/funContainer.hpp"
#include "src/convs.hpp"
#include <QFile>

using namespace std;

void displayHelp()
{
    QFile hfile(":/help.txt");
    hfile.open(QIODevice::ReadOnly | QIODevice::Text);
    std::cout << hfile.readAll().toStdString() << std::endl << "__ ";
    hfile.close();
    getchar();
    exit(0);
}

int main(int c, char ** cargs)
{
    Q_INIT_RESOURCE(res);
    QStringList args;
    for(int i=1;i<c;++i)
        args.append(cargs[i]);

    QString ai="", ac="";

    for(QString arg: args)
    {
        if(arg=="-h")
            displayHelp();
        else if(arg.startsWith("-i")&&!arg.mid(2).isEmpty())
            ai = arg.mid(2);
        else if(arg.startsWith("-c")&&!arg.mid(2).isEmpty())
            ac = arg.mid(2);
        else
        {
            std::cout << "O kij chodzi z argumentem \""+arg.toStdString()+"\"?\n" << std::endl;
            displayHelp();
        }
    }

    std::cout << "\nIn file: " << ai.toStdString() << std::endl;
    std::cout << "Convs file: " << ac.toStdString() << std::endl;

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
