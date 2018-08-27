#include "reg.hpp"
#include <stdexcept>
#include "stack.hpp"

const QStringList Reg::regs = {"r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};

/**
 * Konstruktor, ustawia wartości na podstawie val i zakresu podanego w size.
 */
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

/**
 * Zwraca opis rejestru.
 */
QString Reg::toString() const
{
    QString ret = "[a:"+_a+",w:"+_w+",b:"+_b+"]";
    if(_touched)
        ret.append("*");
    return ret;
}

/**
 * Merguje do siebie wskazany obiekt, jeżeli nastąpiło zwiększenie entropii (któryś z zakresów zmienił się z rejestru na niezdefiniowane dane)
 * zwrócona zostanie wartość true. Ustawia wskaźnik _touched na true.
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
    if(retstate)
        _touched = true;
    return retstate;
}

/**
 * Zwraca wskaźnik _touched.
 */
bool Reg::wasTouched() const
{
    return _touched;
}

/**
 * Pushuje rejestr na podany stos w określonym zakresie.
 * @param stack - referencja na obiekt stosu na którym odłożony zostanie rejestr.
 * @param size - rozmiar odkładanego rejestru (a, w, b).
 */
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

/**
 * Podnosi dane ze stosu do rejestru.
 * @param stack - referencja na obiekt stosu z ktrego pobrać dane.
 * @param size - rozmiar podnoszonej wartości (a, w, b).
 */
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
