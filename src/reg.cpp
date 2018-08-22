#include "reg.hpp"
#include <stdexcept>

const QStringList Reg::regs = {"r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};

Reg::Reg(QString val, QString size)
{
    size = size.toLower();
    _b = val;
    if(size=="b")
    {
        _w = "0";
        _a = "0";
    }
    else if(size=="w")
    {
        _w = val;
        _a = "0";
    }
    else if(size=="a")
    {
        _w = val;
        _a = val;
    }
    else
        throw std::runtime_error("Reg::Reg: nieprawidlowy specyfikator rozmiaru: \""+size.toStdString()+"\".");
}

QString Reg::toString() const
{
    QString ret = "[a:"+_a+",w:"+_w+",b:"+_b+"]";
    if(_touched)
        ret.append("*");
    return ret;
}

/**
 * Merguje do siebie wskazany obiekt, jeąli nastąpiło zwiększenie entropii (któryś z zakresów zmienił się z rejestru na niezdefiniowane dane)
 * zwrócona zostanie wartość true.
 */
bool Reg::merge(const Reg& other)
{
    _touched = true;
    bool retstate = false;
    if((regs.contains(_a))&&(!regs.contains(other._a)))
        retstate = true;
    _a = other._a;
    if((regs.contains(_w))&&(!regs.contains(other._w)))
        retstate = true;
    _w = other._w;
    if((regs.contains(_b))&&(!regs.contains(other._b)))
        retstate = true;
    _b = other._b;
    return retstate;
}

bool Reg::wasTouched() const
{
    return _touched;
}

void Reg::push(Stack& stack, QString size)
{
    if(size=="b")
    {
        stack.pushB(_b);
    }
    else if(size=="w")
    {
        stack.pushB(_b);
        stack.pushB(_w);
    }
    else if(size=="a")
    {
        stack.pushB(_b);
        stack.pushB(_w);
        stack.pushB(_a);
        stack.pushB("0");
    }
    else
        throw std::runtime_error("Reg::push: nieprawidlowy specyfikator rozmiaru: \""+size.toStdString()+"\".");
}

void Reg::pop(Stack& stack, QString size)
{
    _touched = true;
    if(size=="b")
    {
        _b = stack.popB();
        _w = "0";
        _a = "0";
    }
    else if(size=="w")
    {
        _w = stack.popB();
        _b = stack.popB();
        _a = "0";
    }
    else if(size=="a")
    {
        stack.popB();
        _a = stack.popB();
        _w = stack.popB();
        _b = stack.popB();
    }
    else
        throw std::runtime_error("Reg::pop: nieprawidlowy specyfikator rozmiaru: \""+size.toStdString()+"\".");
}
