#include "funContainer.hpp"
#include "Logger.hpp"

/**
 * Dodaje funkcję do listy.
 */
void FunContainer::append(Fun fun)
{
    _funs.append(fun);
}

/**
 * Zwraca referencję do danych.
 */
QList<Fun>& FunContainer::get()
{
    return _funs;
}

/**
 * Po kolei wywołuje symulacje we wszystkich funkcjach. Działa tak długo aż żadna z funkcji z listy nie będzia miała statusu Fun::waiting.
 * Tu się zawiesi przy rekurencji i przy cyklicznych wywołaniach.
 */
void FunContainer::simulate(const Convs *convs)
{
    Logger::Log("Odnaleziono "+QString::number(_funs.size())+" funkcji");
    Logger::Log("Symulowanie funkcji...");

    bool someWaiting = true;

    while(someWaiting)
    {
        someWaiting = false;
        for(Fun& fun: _funs)
        {
            if(fun.state()==Fun::waiting)
            {
                someWaiting = true;
                fun.simulate(this, convs);
            }
        }
    }

    Logger::Log("... zrobione");
}

/**
 * Pobiera funkcję na podstawie jej nazwy. Jeżeli na liście nie ma funkcji o wskazanej nazwie wyrzuci wyjątek.
 */
Fun FunContainer::getByName(QString name) const throw(std::runtime_error)
{
    for(Fun fun: _funs)
    {
        if(fun.name()==name)
            return fun;
    }
    throw std::runtime_error("FunContainer::getByName: brak funkcji o nazwie \""+name.toStdString()+"\"");
}
