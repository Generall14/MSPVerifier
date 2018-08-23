#include "convs.hpp"
#include "Logger.hpp"
#include "fun.hpp"
#include <stdexcept>

Convs::Convs()
{

}

void Convs::loadFromXmlFile(QString adress)
{
    //<TODO>
}

/**
 * Sprawdza zgodność zwracanych przez funkcje rejestrów z konwencją. Zwraca false jeżeli wszystko się zgadza.
 */
bool Convs::checkFun(const Fun& fun) const
{
    for(auto conv: _convs)
    {
        if(conv.name()==fun.getConventionType())
            return conv.checkFun(fun);
    }
    throw std::runtime_error("Convs::checkFun: nie odnaleziono konwencji \""+fun.getConventionType().toStdString()+"\"");
}

QString Convs::toString() const
{
    QString temp;
    for(auto conv: _convs)
        temp += conv.toString()+"\n\n";
    return temp;
}

void Convs::store() const
{
    Logger::WriteFile("_convs.txt", toString());
}
