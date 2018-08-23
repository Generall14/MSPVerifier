#ifndef FUN_HPP
#define FUN_HPP

#include "LineContainer.hpp"
#include <QList>
#include <QMap>
#include "reg.hpp"
#include "stack.hpp"

class FunContainer;
class Line;
class Convs;

class Fun : public LineContainer
{
public:
    Fun(){}
    Fun(QList<Line> lines);

    void simulate(const FunContainer* fc, const Convs* convs);

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
    QString getConventionType() const;

    static Fun prepareFake(QString name, QString conv, uint depth, int ret, const Convs* convs);

private:
    void parse();

    QString _name="__XXXXXX_tu_sie_cos_zesralo";
    simState _state=waiting;
    QString _errorDesc;
    QMap<QString, Reg> _retRegs;
    int _returns=-666;
    QString _convention="???"; //<TODO> odczyt tego
    QString _convState = "ERROR";
    Stack _maxStack;
};

#endif
