#include "conv.hpp"
#include "Logger.hpp"
#include "fun.hpp"
#include <stdexcept>

Conv::Conv(QString name):
    _name(name)
{
    for(int i=4;i<16;i++)
        _types.insert("r"+QString::number(i), typeS());
}

/**
 * Sprawdza zgodność zwracanych przez funkcje rejestrów z konwencją. Zwraca false jeżeli wszystko się zgadza.
 */
bool Conv::checkFun(const Fun& fun, const Stack &stack) const
{
    bool ret = false;
    if(_maxStack>=0)
    {
        if(stack.depth()>_maxStack)
        {
            Logger::LogError("Conv::checkFun: funkcja zajmuje zbyt głęboki stos ("+QString::number(stack.depth())+"), maksymalny "
                        "dopuszczalny to "+QString::number(_maxStack)+".");
            ret = true;
        }
    }
    for(QString key: _types.keys())
    {
        if(!fun.getReturnedRegs().contains(key))
            throw std::runtime_error("Conv::checkFun: brak rejestru "+key.toStdString());

        switch(_types[key]._type)
        {
        case touch:
            break;
        case dontTouch:
            if(fun.getReturnedRegs()[key].wasTouched())
            {
                ret = true;
                Logger::LogError("Conv::checkFun: niezgodność zwracanych rejestrów z przyjętą konwencją, funkcja \""+
                                 fun.name()+"\", rejestr \""+key+"\", konwencja \""+_name+
                                 "\", rejestr nie może być dotykany");
            }
            break;
        case touchAndRestore:
            if(fun.getReturnedRegs()[key]._b!=key)
            {
                ret = true;
                Logger::LogError("Conv::checkFun: niezgodność zwracanych rejestrów z przyjętą konwencją, funkcja \""+
                                 fun.name()+"\", rejestr \""+key+"\", konwencja \""+_name+
                                 "\", rejestr musi być zachowany w zakresie "+_types[key]._size);
                break;
            }
            if((_types[key]._size=="w")||(_types[key]._size=="a"))
            {
                if(fun.getReturnedRegs()[key]._w!=key)
                {
                    ret = true;
                    Logger::LogError("Conv::checkFun: niezgodność zwracanych rejestrów z przyjętą konwencją, funkcja \""+
                                     fun.name()+"\", rejestr \""+key+"\", konwencja \""+_name+
                                     "\", rejestr musi być zachowany w zakresie "+_types[key]._size);
                    break;
                }
                if(_types[key]._size=="a")
                {
                    if(fun.getReturnedRegs()[key]._a!=key)
                    {
                        ret = true;
                        Logger::LogError("Conv::checkFun: niezgodność zwracanych rejestrów z przyjętą konwencją, funkcja \""+
                                         fun.name()+"\", rejestr \""+key+"\", konwencja \""+_name+
                                         "\", rejestr musi być zachowany w zakresie "+_types[key]._size);
                        break;
                    }
                }
            }
            break;
        }
    }
    return ret;
}

QString Conv::name() const
{
    return _name;
}

QString Conv::toString() const
{
    QString temp = "Etykieta: \""+_name+"\", maksymalny poziom stosu: "+QString::number(_maxStack)+" =";
    for(QString key: _types.keys())
    {
        temp += " "+key+":";
        switch(_types[key]._type)
        {
        case touch:
            temp += "free";
            break;
        case dontTouch:
            temp += "forbidden";
            break;
        case touchAndRestore:
            temp += "restored("+_types[key]._size+")";
        }
    }
    return temp;
}

void Conv::set(QString reg, QString type, QString size)
{
    if(!_types.contains(reg))
        throw std::runtime_error("Conv::set: brak rejestru \""+reg.toStdString()+"\"");
    typeS temp;
    if(type=="free")
        temp._type=touch;
    else if(type=="forbidden")
        temp._type=dontTouch;
    else if(type=="restored")
        temp._type=touchAndRestore;
    temp._size = size;
    _types[reg]=temp;
}

QMap<QString, Reg> Conv::prepareReturnedRegs() const
{
    QMap<QString, Reg> temp;
    for(auto key: _types.keys())
        temp.insert(key, prepare(_types[key], key));
    return temp;
}

Reg Conv::prepare(const typeS &typ, QString reg) const
{
    Reg temp;
    switch(typ._type)
    {
    case touch:
        temp._a="?";
        temp._w="?";
        temp._b="?";
        temp._touched=true;
        break;
    case dontTouch:
        temp._a=reg;
        temp._w=reg;
        temp._b=reg;
        break;
    case touchAndRestore:
        if(typ._size=="a")
        {
            temp._a=reg;
            temp._w=reg;
            temp._b=reg;
        }
        else if(typ._size=="w")
        {
            temp._a="?";
            temp._w=reg;
            temp._b=reg;
        }
        else if(typ._size=="b")
        {
            temp._a="?";
            temp._w="?";
            temp._b=reg;
        }
        temp._touched=true;
    }
    return temp;
}
