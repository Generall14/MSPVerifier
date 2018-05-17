#include "Logger.hpp"
#include <QFile>
#include <QTextStream>
#include <exception>
#include <iostream>

const QString Logger::dir = "./outputs/";
const QString Logger::errorfile = "errors.txt";
const QString Logger::warningfile = "warnings.txt";

void Logger::ClearFiles()
{
    QFile f1(dir+errorfile);
    f1.open(QIODevice::Truncate | QIODevice::ReadWrite);
    f1.close();

    QFile f2(dir+warningfile);
    f2.open(QIODevice::Truncate | QIODevice::ReadWrite);
    f2.close();
}

void Logger::LogError(QString txt)
{
    QString text = "ERROR: \n" + txt + "\n\n";
    Log(text, dir+errorfile);
    std::cout << text.toStdString();
}

void Logger::LogWarning(QString txt)
{
    QString text = "WARNING: \n" + txt + "\n\n";
    Log(text, dir+warningfile);
    std::cout << text.toStdString();
}

void Logger::Log(QString txt, QString adr)
{
    QFile file(adr);
    if(!file.open(QIODevice::Append | QIODevice::Text))
        throw std::runtime_error( QString("Nie można pisac do pliku "+adr).toStdString() );

    QTextStream str(&file);
    str << txt;

    file.close();
}
