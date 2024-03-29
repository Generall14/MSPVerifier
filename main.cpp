#include <iostream>
#include <QString>
#include <stdexcept>
#include <QFile>
#include <clocale>
#include "src/MSPVerifier.hpp"

void displayHelp()
{
    QFile hfile(":/help.txt");
    hfile.open(QIODevice::ReadOnly | QIODevice::Text);
    std::cout << hfile.readAll().toStdString() << std::endl << "-->";
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
    bool quiet=false;

//    system("chcp 65000");
//    system("chcp");
//    setlocale(LC_ALL,"pl_PL.UTF-8");
//    setlocale(LC_ALL,".852");
//    printf ("Locale is: %s\n", setlocale(LC_ALL,NULL) );
//    std::cout << "ąćźół\n";

    for(QString arg: args)
    {
        if(arg=="-h")
            displayHelp();
        else if(arg.startsWith("-i")&&!arg.mid(2).isEmpty())
            ai = arg.mid(2);
        else if(arg.startsWith("-c")&&!arg.mid(2).isEmpty())
            ac = arg.mid(2);
        else if(arg.startsWith("-q"))
            quiet = true;
        else
        {
            std::cout << "O kij chodzi z argumentem \""+arg.toStdString()+"\"?\n" << std::endl;
            displayHelp();
        }
    }

    if((ai.isEmpty())||(ac.isEmpty()))
    {
        std::cout << "Brak wymaganych argumentów\n";
        displayHelp();
    }

    std::cout << "\nIn file: \"" << ai.toStdString() << "\"\n";
    std::cout << "Convs file: \"" << ac.toStdString() << "\"\n\n";

    MSPV::verify(ai, ac, quiet); // tu się dzieje cała magia

    std::cout << "\n-->";
    getchar();
    return 0;
}
