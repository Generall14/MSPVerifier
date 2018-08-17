#include "core.hpp"

Core::Core(QString name):
    _name(name),
    _stack(name)
{
    for(auto r: Reg::regs)
        _regs.insert(r, Reg(r, "a"));
}

/**
 * Merguje do siebie wskazany obiekt, jeżeli nastąpiło zwiększenie entropii (w którymś z rejestrów) zwrócona zostanie wartość true. Jeżeli
 * stosy okażą się niezgodne - zgłoszoy zostanie wyjątek.5
 */
bool Core::merge(const Core& other)
{
    // <TODO> sprawdz stosy - niezgodne - throw
    // <TODO> wciagnij i sprawdz
}
