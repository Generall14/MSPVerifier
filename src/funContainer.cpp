#include "funContainer.hpp"

void FunContainer::append(Fun fun)
{
    _funs.append(fun);
}

QList<Fun>& FunContainer::get()
{
    return _funs;
}
