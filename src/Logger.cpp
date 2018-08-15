#include "Logger.hpp"
#include <QFile>
#include <QTextStream>
#include <exception>
#include <iostream>
#include <QDir>
#include <QFileInfo>

const QString Logger::dir = "./outputs/";
const QString Logger::errorfile = "errors.txt";
const QString Logger::warningfile = "warnings.txt";
const QString Logger::statusfile = "log.txt";

void Logger::ClearFiles()
{
    QDir qdir(QFileInfo(dir).dir());
    if(!qdir.exists())
        qdir.mkpath("./");

    QFile f1(dir+errorfile);
    f1.open(QIODevice::Truncate | QIODevice::ReadWrite);
    f1.close();

    QFile f2(dir+warningfile);
    f2.open(QIODevice::Truncate | QIODevice::ReadWrite);
    f2.close();

    QFile f3(dir+statusfile);
    f3.open(QIODevice::Truncate | QIODevice::ReadWrite);
    f3.close();
}

void Logger::WriteFile(QString file, QString text)
{
    QFile f1(dir+file);
    f1.open(QIODevice::Truncate | QIODevice::ReadWrite);
    f1.close();
    Log(text, dir+file);
}

void Logger::AppendFile(QString file, QString text)
{
    QFile f1(dir+file);
    f1.open(QIODevice::Append | QIODevice::ReadWrite);
    f1.close();
    Log(text, dir+file);
}

void Logger::LogError(QString txt)
{
    QString text = "ERROR: \n" + txt + "\n\n";
    Log(text, dir+errorfile);
    Log(text, dir+statusfile);
    std::cout << text.toStdString();
}

void Logger::LogWarning(QString txt)
{
    QString text = "WARNING: \n" + txt + "\n\n";
    Log(text, dir+warningfile);
    Log(text, dir+statusfile);
    std::cout << text.toStdString();
}

void Logger::Log(QString txt)
{
    QString text = txt + "\n\n";
    Log(text, dir+statusfile);
    std::cout << text.toStdString();
}

void Logger::Log(QString txt, QString adr)
{
    QDir qdir(QFileInfo(adr).dir());
    if(!qdir.exists())
        qdir.mkpath("./");

    QFile file(adr);
    if(!file.open(QIODevice::Append | QIODevice::Text))
        throw std::runtime_error( QString("Nie można pisac do pliku "+adr).toStdString() );

    QTextStream str(&file);
    str << txt;

    file.close();
}
