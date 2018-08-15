#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <QString>

class Logger
{
public:
    static void ClearFiles();
    static void LogError(QString txt);
    static void LogWarning(QString txt);
    static void Log(QString txt);

    static void WriteFile(QString file, QString text);
    static void AppendFile(QString file, QString text);

private:
    Logger(){}
    static const QString dir;
    static const QString errorfile;
    static const QString warningfile;
    static const QString statusfile;

    static void Log(QString txt, QString adr);
};

#endif
