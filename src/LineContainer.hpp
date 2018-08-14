#ifndef LINECONTAINER_HPP
#define LINECONTAINER_HPP

#include <QList>
#include "Line.hpp"

class LineContainer
{
public:
    LineContainer(QList<Line> lines);
    LineContainer(){}

    const QList<Line>& get();

    QString toString() const;
    QString toSString() const;

private:
    QList<Line> _lines;
};

#endif
