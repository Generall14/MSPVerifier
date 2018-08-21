#ifndef CORE_HPP
#define CORE_HPP

#include <QString>
#include <QMap>
#include "reg.hpp"
#include "stack.hpp"
#include "Line.hpp"

class Core
{
public:
    Core(QString name);
    Core(const Core& other);
    Core& operator=(const Core& other);

    bool merge(const Core& other);
    bool loadInstruction(const Line& line);

    QString toString() const;

    static const QStringList jumps;
    static const QStringList jumpsIf;
    static const QStringList rets;

private:
    QString _name="";
    Stack _stack;
    QMap<QString, Reg> _regs;

    static const QStringList biMArgs;
    static const QStringList singleMArgs;
    static const QStringList transparentArgs;
};

#endif
