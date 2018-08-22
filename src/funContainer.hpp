#ifndef FUNCONTAINER_HPP
#define FUNCONTAINER_HPP

#include "fun.hpp"
#include <stdexcept>
#include <QList>

class FunContainer
{
public:
    FunContainer(){}

    void append(Fun fun);
    void simulate();
    Fun getByName(QString name) const throw(std::runtime_error);

    QList<Fun>& get();

private:
    QList<Fun> _funs;
};

#endif
