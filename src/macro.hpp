#ifndef MACRO_HPP
#define MACRO_HPP

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

    QStringList _args;
    QStringList _lines;
    QString _name;
};

#endif
