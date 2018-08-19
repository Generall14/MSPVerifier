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

    bool merge(const Core& other);
    bool loadInstruction(const Line& line);

    QString toString() const;

private:
    QString _name;
    Stack _stack;
    QMap<QString, Reg> _regs;
};

#endif
