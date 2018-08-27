#ifndef MACRO_HPP
#define MACRO_HPP

/**
  * @class Macro
  * @author Wojciech Kogut
  *
  * Klasa reprezentuje makro asemblera, posiada metodę statyczną ładującą makra z obiektu File oraz metodę rozwijającą makra w File.
  */

#include <QList>
#include <QMap>

class Line;
class File;

class Macro
{
public:
    static QList<Macro> loadMacros(File& file);
    void applyMacro(File& file);

private:
    Macro(){}

    QStringList _args; /**<Lista argumentów.*/
    QStringList _lines; /**<Tekst makra.*/
    QString _name; /**<Nazwa makra.*/
};

#endif
