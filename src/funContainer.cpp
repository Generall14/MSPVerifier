#include "funContainer.hpp"
#include "Logger.hpp"

void FunContainer::append(Fun fun)
{
    _funs.append(fun);
}

QList<Fun>& FunContainer::get()
{
    return _funs;
}

// To się zawiesi przy jakielkolwiek rekurencji.
void FunContainer::simulate(const Convs *convs)
{
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
    //<TODO> zapis do pliku? tutaj czy w File po zmianie statusu z waiting?
}

Fun FunContainer::getByName(QString name) const throw(std::runtime_error)
{
    for(Fun fun: _funs)
    {
        if(fun.name()==name)
            return fun;
    }
    throw std::runtime_error("FunContainer::getByName: brak funkcji o nazwie \""+name.toStdString()+"\"");
}
