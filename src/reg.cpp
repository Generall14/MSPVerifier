#include "reg.hpp"
#include <stdexcept>

const QStringList Reg::regs = {"r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};

Reg::Reg(QString val, QString size)
{
    size = size.toLower();
    _b = val;
    if(size=="b")
    {
        _w = "?";
        _a = "?";
    }
    else if(size=="w")
    {
        _w = val;
        _a = "";
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
    return "[a:"+_a+",w:"+_w+",b:"+_b+"]";
}

/**
 * Merguje do siebie wskazany obiekt, jeąli nastąpiło zwiększenie entropii (któryś z zakresów zmienił się z rejestru na niezdefiniowane dane)
 * zwrócona zostanie wartość true.
 */
bool Reg::merge(const Reg& other)
{
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
