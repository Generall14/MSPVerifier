#include "LineContainer.hpp"

LineContainer::LineContainer(QList<Line> lines):
    _lines(lines)
{

}

QList<Line>& LineContainer::get()
{
    return _lines;
}

QString LineContainer::toString() const
{
    QString temp;
    for(auto line: _lines)
        temp += line.toString()+"\n";
    return temp;
}

QString LineContainer::toSString() const
{
    QString temp;
    for(auto line: _lines)
        temp += line.toSString()+"\n";
    return temp;
}
