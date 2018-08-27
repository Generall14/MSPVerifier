#include "segment.hpp"
#include <stdexcept>
#include "Line.hpp"

/**
 * @param lines - Lista lini z których składa się segment.
 * @param type - Typ segmentu.
 */
Segment::Segment(QList<Line> lines, QString type):
    _type(type)
{
    _lines.append(lines);

    if(_lines.isEmpty())
        throw std::runtime_error("Segment::Segment: brak przekazanych lini");
}

/**
 * Zwraca typ segmentu.
 */
QString Segment::type() const
{
    return _type;
}
