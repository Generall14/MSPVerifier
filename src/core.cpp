#include "core.hpp"
#include "Logger.hpp"
#include "Line.hpp"
#include <qdebug.h>

const QStringList Core::biMArgs = {"add", "addx", "sub", "subx", "bic", "bicx", "bis", "bisx", "xor", "xorx",
                                  "and", "andx", "mov", "movx", "addc", "subc"};
const QStringList Core::singleMArgs = {"swpb", "inc", "incx", "dec", "decx", "rla", "rra", "adc", "rrc", "rlc", "rlam", "inv", "adc",
                                      "sxt"};
const QStringList Core::transparentArgs = {"cmp", "cmpx", "tst", "tstx", "bit", "bitx", "nop", "eint", "dint", "setz", "clrz", "clrc",
                                           "setc"};
const QStringList Core::jumps = {"jmp"};
const QStringList Core::jumpsIf = {"jnz", "jz", "jc", "jnc", "jn", "jl"};
const QStringList Core::rets = {"ret", "reta", "reti"};
const QStringList Core::calls = {"call", "calla"};
const QStringList Core::pushes = {"push", "pushx"};
const QStringList Core::pops = {"pop", "popx"};
const QStringList Core::adds = {"add", "addx"};
const QStringList Core::subs = {"sub", "subx"};

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
    _regs(other._regs),
    _returns(other._returns)
{

}

Core& Core::operator=(const Core& other)
{
    if(this==&other)
        return *this;

    _name=other._name;
    _stack=other._stack;
    _regs=other._regs;
    _returns=other._returns;

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
    if(_returns==0)
        _returns=other._returns;
    if(_returns!=other._returns)
        throw std::runtime_error("Core::merge: różne poziomy powrotu");

    return rets;
}

/**
 * Ładuje do siebie wskazane rejestry (jeżeli we wskazanej mapie rejestry były modyfikowane - załąduje je do swoich).
 */
void Core::loadReturnedRegs(const QMap<QString, Reg>& regs)
{
    for(auto key: _regs.keys())
    {
        if(!regs.contains(key))
            continue;
        if(!regs[key].wasTouched())
            continue;
        _regs[key]._touched = true;
        if(regs[key]._a!=key)
            _regs[key]._a = regs[key]._a;
        if(regs[key]._w!=key)
            _regs[key]._w = regs[key]._w;
        if(regs[key]._b!=key)
            _regs[key]._b = regs[key]._b;
    }
}

/**
 * Dodaje wskazaną funkcję jako wywoływaną (nie inkrementuje poziomu stosu).
 */
void Core::call(const Fun& fun)
{
    _stack.call(fun);
}

/**
 * Zwraca opis obiektu.
 */
QString Core::toString() const
{
    QString temp = _stack.toSString()+"'";
    for(QString key: _regs.keys())
        temp += "'"+key+_regs[key].toString();
    return temp;
}

/**
 * Symuluje wskazaną instrukcję, jeżeli nastąpiło zwiększenie entropii (w którymś z rejestrów) zwrócona zostanie wartość true. Jeżeli
 * stosy okażą się niezgodne - zgłoszoy zostanie wyjątek.
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

    // Instrukcje powrotu z procedury.
    if(rets.contains(line.getInstruction(), Qt::CaseInsensitive))
    {
        if(_stack.depth())
            throw std::runtime_error("Core::loadInstruction: próba powrotu z procedury na niezerozym poziomie"
                                     " lokalnego stosu");

        if((line.getInstruction()=="ret")||(line.getInstruction()=="reti"))
            _returns = -2;
        else if(line.getInstruction()=="reta")
            _returns = -4;
        else
            throw std::runtime_error("Core::loadInstruction: wtf, nieznany return?");
        return false;
    }

    // Instrukcje wołania procedur, tutaj dodawane tylko symboliczne śmieci na stos.
    if(calls.contains(line.getInstruction(), Qt::CaseInsensitive))
    {
        if(line.getInstruction()=="call")
        {
            _stack.pushB("call");
            _stack.pushB("call");
        }
        else if(line.getInstruction()=="calla")
        {
            _stack.pushB("calla");
            _stack.pushB("calla");
            _stack.pushB("calla");
            _stack.pushB("calla");
        }
        else
            throw std::runtime_error("Core::loadInstruction: wtf, nieznany call?");
        return false;
    }

    // Wykrywanie autoinkrementacji:
    for(QString arg: line.getArguments())
    {
        if(arg.startsWith("@")&&(arg.endsWith("+")))
        {
            QString r = arg.toLower().remove("@").remove("+");
            if(_regs.contains(r))
            {
                _regs[r].merge(Reg("?", "a"));
            }
        }
    }

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
        // Sprawdź czy to modyfikowanie SP przez dodawanie lub odejmowanie <TODO> to się aż prosi o zrefaktoryzowanie
        if(adds.contains(line.getInstruction(), Qt::CaseInsensitive))
        {
            if(line.getArguments().at(1).toLower()=="sp")
            {
                if(!line.getArguments().at(0).startsWith("#"))
                {
                    Logger::LogError("Nie wiem co z tym zrobić: "+line.toString());
                    return false;
                }
                int val;
                bool ok;
                if(line.getArguments().at(0).mid(1).startsWith("0x"))
                    val = line.getArguments().at(0).mid(1).toInt(&ok, 16);
                else
                    val = line.getArguments().at(0).mid(1).toInt(&ok, 10);
                if(!ok)
                {
                    Logger::LogError("Nie wiem co z tym zrobić: "+line.toString());
                    return false;
                }
                while(val--)
                    _stack.popB();
            }
        }
        else if(subs.contains(line.getInstruction(), Qt::CaseInsensitive))
        {
            if(line.getArguments().at(1).toLower()=="sp")
            {
                if(!line.getArguments().at(0).startsWith("#"))
                {
                    Logger::LogError("Nie wiem co z tym zrobić: "+line.toString());
                    return false;
                }
                int val;
                bool ok;
                if(line.getArguments().at(0).mid(1).startsWith("0x"))
                    val = line.getArguments().at(0).mid(1).toInt(&ok, 16);
                else
                    val = line.getArguments().at(0).mid(1).toInt(&ok, 10);
                if(!ok)
                {
                    Logger::LogError("Nie wiem co z tym zrobić: "+line.toString());
                    return false;
                }
                while(val--)
                    _stack.pushB("??");
            }
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

    // Instrukcja pushująca na stos
    if(pushes.contains(line.getInstruction(), Qt::CaseInsensitive))
    {
        if(line.getArguments().size()<1)
            Logger::LogError("Za mało argumentów w "+line.toString());
        if(_regs.contains(line.getArguments().at(0).toLower()))
            _regs[line.getArguments().at(0).toLower()].push(_stack, line.getInstructionSize());
        else
            _stack.pushRandomStuff(line.getInstructionSize());
        return false;
    }

    // Instrukcja wciągająca ze stosu
    if(pops.contains(line.getInstruction(), Qt::CaseInsensitive))
    {
        if(line.getArguments().size()<1)
        {
            Logger::LogError("Za mało argumentów w "+line.toString());
            return false;
        }
        if(_regs.contains(line.getArguments().at(0).toLower()))
        {
            Reg treg = _regs[line.getArguments().at(0).toLower()];
            treg.pop(_stack, line.getInstructionSize());
            return _regs[line.getArguments().at(0).toLower()].merge(treg);
        }
        else
            _stack.popRandomStuff(line.getInstructionSize());
        return false;
    }

    // WTF? Nic z powyższych.
    Logger::LogError("Nieznana instrukcja w "+line.toString());
    return false;
}

/**
 * Zwraca poziom powrotu z funkcji (-2 dla ret, -4 dla rata).
 */
int Core::returns() const
{
    return _returns;
}
