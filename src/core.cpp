#include "core.hpp"

Core::Core(QString name):
    _name(name),
    _stack(name)
{
    for(auto r: Reg::regs)
        _regs.insert(r, Reg(r, "a"));
}

Core::Core(const Core& other):
    _name(other._name),
    _stack(other._stack),
    _regs(other._regs)
{

}

Core& Core::operator=(const Core& other)
{
    if(this==&other)
        return *this;

    _name=other._name;
    _stack=other._stack;
    _regs=other._regs;

    return *this;
}

/**
 * Merguje do siebie wskazany obiekt, jeżeli nastąpiło zwiększenie entropii (w którymś z rejestrów) zwrócona zostanie wartość true. Jeżeli
 * stosy okażą się niezgodne - zgłoszoy zostanie wyjątek.5
 */
bool Core::merge(const Core& other)
{
    // <TODO> sprawdz stosy - niezgodne - throw
    // <TODO> wciagnij i sprawdz

    return true;
}

QString Core::toString() const
{
    QString temp = _stack.toString()+"'";
    for(QString key: _regs.keys())
        temp += "'"+key+_regs[key].toString();
    return "core"; //<TODO>
}

/**
 * Symuluje wskazaną instrukcję, jeżeli nastąpiło zwiększenie entropii (w którymś z rejestrów) zwrócona zostanie wartość true. Jeżeli
 * stosy okażą się niezgodne - zgłoszoy zostanie wyjątek.5
 */
bool Core::loadInstruction(const Line& line)
{

}
