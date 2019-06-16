#include "convs.hpp"
#include "Logger.hpp"
#include "fun.hpp"
#include <stdexcept>
#include <pugixml.hpp>
#include "conv.hpp"

Convs::Convs()
{

}

/**
 * Ładuje dane ze wskazanego pliku.
 */
void Convs::loadFromXmlFile(QString adress)
{
    // Otwieranie pliku xml:
    pugi::xml_document file;
    pugi::xml_parse_result result =  file.load_file(adress.toStdString().c_str());

    if(!result)
        throw std::runtime_error("Convs::loadFromXmlFile: nie mozna otworzyc pliku XML \""+adress.toStdString()+
                                 "\": "+result.description()+", offset: "+
                                 QString::number(result.offset).toStdString()+" B");

    pugi::xml_node mainnode = file.root().child("mainnode");
    if(mainnode.empty())
        throw std::runtime_error("Convs::loadFromXmlFile: brak mannode");

    for(auto child: mainnode.children())
        iterateInConvs(child);
}

/**
 * Sprawdza zgodność zwracanych przez funkcje rejestrów z konwencją. Zwraca false jeżeli wszystko się zgadza.
 */
bool Convs::checkFun(const Fun& fun, const Stack& stack) const
{
    for(auto conv: _convs)
    {
        if(conv.name()==fun.getConventionType())
            return conv.checkFun(fun, stack);
    }
    throw std::runtime_error("Convs::checkFun: nie odnaleziono konwencji \""+fun.getConventionType().toStdString()+"\"");
}

/**
 * Zwraca opis obiektu.
 */
QString Convs::toString() const
{
    QString temp;
    for(auto conv: _convs)
        temp += conv.toString()+"\n\n";
    return temp;
}

/**
 * Zapisuje liste zawartych konwencji w pliku outputs/_convs.txt.
 */
void Convs::store() const
{
    Logger::WriteFile("_convs.txt", toString());
}

/**
 * Metoda pomocnicza do czytania pliku xml.
 */
void Convs::iterateInConvs(pugi::xml_node& node)
{
    try
    {
        if(QString(node.name())!="conv")
            return;
        Conv temp(node.attribute("lab").as_string("??"));

        temp._maxStack = node.attribute("maxstack").as_int(-1);

        for(pugi::xml_node child: node.children())
        {
            QString reg = child.attribute("name").as_string("??");
            QString type = child.attribute("type").as_string("??");
            QString size = child.attribute("size").as_string("??");
            temp.set(reg, type, size);
        }
        _convs.append(temp);
    }
    catch(...)
    {
        throw std::runtime_error("Convs::iterateInConvs: jakiś błąd czytanie pliku xml.");
    }
}

/**
 * Zwraca spreparowane rejestry zwracane przez funkcję działającą na podstawie konwencji o wskazanej nazwie.
 */
QMap<QString, Reg> Convs::prepareReturnedRegs(QString convname) const
{
    for(auto conv: _convs)
    {
        if(conv.name()==convname)
            return conv.prepareReturnedRegs();
    }
    throw std::runtime_error("Convs::checkFun: nie odnaleziono konwencji \""+convname.toStdString()+"\"");
}
