#ifndef FUN_HPP
#define FUN_HPP

#include "LineContainer.hpp"
#include <QList>
#include <QMap>
#include "reg.hpp"

class FunContainer;
class Stack;
class Line;

class Fun : public LineContainer
{
public:
    Fun(){}
    Fun(QList<Line> lines);

    void simulate(const FunContainer* fc);

    enum simState
    {
        waiting,
        done,
        error
    };

    QString toString() const;
    simState state() const;
    QString name() const;
    Stack getMaxStack() const;
    int getReturnedLevel() const;
    QMap<QString, Reg> getReturnedRegs() const;

    //<TODO> metoda zwracająca stan rdzenia
    //<TODO> metoda zwracające maksymalne wywołania?

private:
    void parse();

    QString _name="__XXXXXX_tu_sie_cos_zesralo";
    simState _state=waiting;
    QString _errorDesc;
    QMap<QString, Reg> _retRegs;
};

#endif
