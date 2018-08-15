#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include "LineContainer.hpp"
#include "Line.hpp"
#include <QList>

class Segment : public LineContainer
{
public:
    Segment(QList<Line> lines, QString type);

    QString type() const;
    QString section() const;

private:
    QString _type;
};

#endif