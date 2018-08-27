#ifndef FUNCONTAINER_HPP
#define FUNCONTAINER_HPP

/**
  * @class FunContainer
  * @author Wojciech Kogut
  *
  * Klasa zawiera listę funkcji (zapisanych jako obiekty Fun), posiada metodę simulate(...) która analizuje wszystkie funkcje na liście.
  */

#include "fun.hpp"
#include <stdexcept>
#include <QList>

class Convs;

class FunContainer
{
public:
    FunContainer(){}

    void append(Fun fun);
    void simulate(const Convs* convs);
    Fun getByName(QString name) const throw(std::runtime_error);

    QList<Fun>& get();

private:
    QList<Fun> _funs;
};

#endif
