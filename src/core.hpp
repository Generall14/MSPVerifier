#ifndef CORE_HPP
#define CORE_HPP

#include <QString>
#include <QMap>
#include "reg.hpp"
#include "stack.hpp"

class Line;
class Fun;

class Core
{
    friend class Fun;
public:
    Core(QString name);
    Core(const Core& other);
    Core& operator=(const Core& other);

    bool merge(const Core& other);
    bool loadInstruction(const Line& line);
    void loadReturnedRegs(const QMap<QString, Reg>& regs);
    void call(const Fun& fun);
    int returns() const;

    QString toString() const;

    static const QStringList jumps;
    static const QStringList jumpsIf;
    static const QStringList rets;
    static const QStringList calls;

private:
    QString _name="";
    Stack _stack;
    QMap<QString, Reg> _regs;
    int _returns=0;

    static const QStringList biMArgs;
    static const QStringList singleMArgs;
    static const QStringList transparentArgs;
    static const QStringList pushes;
    static const QStringList pops;
};

#endif
