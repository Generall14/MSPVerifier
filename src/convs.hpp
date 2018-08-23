#ifndef CONVS_HPP
#define CONVS_HPP

#include "osrc/pugixml.hpp"
#include "conv.hpp"

class Fun;

class Convs
{
public:
    Convs();
    void loadFromXmlFile(QString adress);

    bool checkFun(const Fun& fun) const;

    QString toString() const;
    void store() const;

private:
    void iterateInConvs(pugi::xml_node& node);

    QList<Conv> _convs;
};

#endif
