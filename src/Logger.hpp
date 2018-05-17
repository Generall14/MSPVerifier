#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <QString>

class Logger
{
public:
    static void ClearFiles();
    static void LogError(QString txt);
    static void LogWarning(QString txt);

private:
    Logger(){}
    static const QString dir;
    static const QString errorfile;
    static const QString warningfile;

    static void Log(QString txt, QString adr);
};

#endif
