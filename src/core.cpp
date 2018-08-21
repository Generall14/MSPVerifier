#include "core.hpp"
#include "Logger.hpp"

const QStringList Core::biMArgs = {"add", "addx", "sub", "subx", "bic", "bicx", "bis", "bisx", "xor", "xorx",
                                  "and", "andx", "mov", "movx"};
const QStringList Core::singleMArgs = {"swpb"};
const QStringList Core::transparentArgs = {"cmp", "cmpx", "tst", "tstx", "bit", "bitx", "nop"};
const QStringList Core::jumps = {"jmp"};
const QStringList Core::jumpsIf = {"jnz", "jz", "jc", "jnc"};
const QStringList Core::rets = {"ret"};

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
 * stosy okażą się niezgodne - zgłoszoy zostanie wyjątek.
 */
bool Core::merge(const Core& other)
{
    int rets = 0;
    rets += _stack.merge(other._stack);
    for(QString key: _regs.keys())
        rets += _regs[key].merge(other._regs[key]);

    return rets;
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
    // Jeżeli linie nie zawiera instrukcji - nic się nie zmienia.
    if(line.getInstruction().isEmpty())
        return false;

    // Jeżeli linia zawiera instrukcjię nic nie robiącą z rdzeniem - ret;
    if(transparentArgs.contains(line.getInstruction(), Qt::CaseInsensitive))
        return false;
    if(jumpsIf.contains(line.getInstruction(), Qt::CaseInsensitive))
        return false;

    // <TODO> instrukcje specjalne - pop, push, grzebanie w sp

    // Jeżeli linia zawiera instrukcję dwu argumentową oraz drugi argument jest rejestrem - modyfikuj
    // rejestr.
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

    // Jeżeli linia zawiera instrukcję jedno argumentową oraz argument jest rejestrem - modyfikuj rejestr.
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

    // WTF? Nic z powyższych.
    Logger::LogError("Nieznana instrukcja w "+line.toString());
    return false;
}
