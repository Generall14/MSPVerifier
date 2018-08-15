#ifndef FUNCONTAINER_HPP
#define FUNCONTAINER_HPP

#include "fun.hpp"
#include <QList>

class FunContainer
{
public:
    FunContainer(){}

    void append(Fun fun);

    QList<Fun>& get();

private:
    QList<Fun> _funs;
};

#endif
