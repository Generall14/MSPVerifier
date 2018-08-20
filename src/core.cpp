#include "core.hpp"
#include "Logger.hpp"

const QStringList Core::mov = {"mov", "movx"};
const QStringList Core::biMArgs = {"add", "addx", "sub", "subx", "bic", "bicx", "bis", "bisx", "xor", "xorx",
                                  "and", "andx"};
const QStringList Core::singleMArgs = {"swpb"};
const QStringList Core::transparentArgs = {"cmp", "cmpx", "tst", "tstx", "bit", "bitx", "nop"};

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
    return temp;
}

/**
 * Symuluje wskazaną instrukcję, jeżeli nastąpiło zwiększenie entropii (w którymś z rejestrów) zwrócona zostanie wartość true. Jeżeli
 * stosy okażą się niezgodne - zgłoszoy zostanie wyjątek.5
 */
bool Core::loadInstruction(const Line& line)
{
    if(line.getInstruction().isEmpty())
        return false;

    if(transparentArgs.contains(line.getInstruction(), Qt::CaseInsensitive))
        return false;

    if(mov.contains(line.getInstruction(), Qt::CaseInsensitive))
    {
        if(line.getArguments().size()<2)
        {
            Logger::LogError("Za mało argumentów w "+line.toString());
            return false;
        }
        if(_regs.contains(line.getArguments().at(1).toLower()))
        {
            QString fa = "?";
            if(_regs.contains(line.getArguments().at(0)))
                fa = line.getArguments().at(0).toLower();
            return _regs[line.getArguments().at(1).toLower()].merge(Reg(fa, line.getInstructionSize()));
        }
        return false;
    }

    if(biMArgs.contains(line.getInstruction(), Qt::CaseInsensitive))
    {
        if(line.getArguments().size()<2)
        {
            Logger::LogError("Za mało argumentów w "+line.toString());
            return false;
        }
        if(_regs.contains(line.getArguments().at(1).toLower()))
        {
            return _regs[line.getArguments().at(1).toLower()].merge(Reg("?", line.getInstructionSize()));
        }
        return false;
    }

    if(singleMArgs.contains(line.getInstruction(), Qt::CaseInsensitive))
    {
        if(line.getArguments().size()<1)
        {
            Logger::LogError("Za mało argumentów w "+line.toString());
            return false;
        }
        if(_regs.contains(line.getArguments().at(0).toLower()))
        {
            return _regs[line.getArguments().at(0).toLower()].merge(Reg("?", line.getInstructionSize()));
        }
        return false;
    }

    Logger::LogError("Nieznana instrukcja w "+line.toString());
    return false;
}
