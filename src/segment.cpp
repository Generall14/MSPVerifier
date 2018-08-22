#include "segment.hpp"
#include <stdexcept>
#include <QDebug>
#include "Line.hpp"

Segment::Segment(QList<Line> lines, QString type):
    _type(type)
{
    _lines.append(lines);

    if(_lines.isEmpty())
        throw std::runtime_error("Segment::Segment: brak przekazanych lini");
}

QString Segment::type() const
{
    return _type;
}
