#ifndef LOGGER_HPP
#define LOGGER_HPP

/**
  * @class Logger
  * @author Wojciech Kogut
  *
  * Klasa służy do zachowywania danych w plikach.
  */

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
    static const QString dir; /**<Katalog bazowy plików wyjściowych.*/
    static const QString errorfile; /**<Nazwa pliku z errorami.*/
    static const QString warningfile; /**<Nazwa plik z warningami.*/
    static const QString statusfile; /**<Nazwa pliku z logami.*/

    static void Log(QString txt, QString adr);
};

#endif
