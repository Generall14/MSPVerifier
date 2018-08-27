#ifndef LINECONTAINER_HPP
#define LINECONTAINER_HPP

/**
  * @class LineContainer
  * @author Wojciech Kogut
  *
  * Klasa jest bazą dla wszystkich klas zawierających listę obiektów Line (reprezentujących pliki, funkcje, sekcje itp.). Jedynym jej zadaniem
  * jest przechowywanie listy obiektów Line.
  */

#include <QList>
#include "Line.hpp"

class LineContainer
{
public:
    LineContainer(QList<Line> lines);
    LineContainer(){}

    QList<Line>& get();

    QString toString() const;
    QString toSString() const;

protected:
    QList<Line> _lines; /**<Zbiór linii tekst*/
};

#endif
