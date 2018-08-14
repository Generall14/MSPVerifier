#ifndef LINECONTAINER_HPP
#define LINECONTAINER_HPP

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
    QList<Line> _lines;
};

#endif
