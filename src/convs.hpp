#ifndef CONVS_HPP
#define CONVS_HPP

/**
  * @class Convs
  * @author Wojciech Kogut
  *
  * Klasa zawiera listę konwencji. Posiada metodę wczytująca konwencje z pliku konfiguracyjnego *.xml.
  */

#include <pugixml.hpp>
#include "conv.hpp"

class Fun;
class Reg;

class Convs
{
public:
    Convs();
    void loadFromXmlFile(QString adress);

    bool checkFun(const Fun& fun, const Stack &stack) const;
    QMap<QString, Reg> prepareReturnedRegs(QString convname) const;

    QString toString() const;
    void store() const;

private:
    void iterateInConvs(pugi::xml_node& node);

    QList<Conv> _convs; /**<Lista konwencji.*/
};

#endif
