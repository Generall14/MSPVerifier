#ifndef FUN_HPP
#define FUN_HPP

#include "LineContainer.hpp"
#include <QList>
#include "Line.hpp"

class FunContainer;
class Stack;

class Fun : public LineContainer
{
public:
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

    //<TODO> metoda zwracająca stan rdzenia
    //<TODO> metoda zwracające maksymalne wywołania?

private:
    void parse();

    QString _name="__XXXXXX_tu_sie_cos_zesralo";
    simState _state=waiting;
};

#endif
