#ifndef SEGMENTLIST_HPP
#define SEGMENTLIST_HPP

#include "segment.hpp"
#include <QList>
#include <QMap>

class SegmentList
{
public:
    void append(Segment seg);
    QMap<QString, QList<Segment> >& get();

    void store();

private:
    QMap<QString, QList<Segment> > _seg;
};

#endif
