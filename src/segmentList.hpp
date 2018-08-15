#ifndef SEGMENTLIST_HPP
#define SEGMENTLIST_HPP

#include "segment.hpp"
#include "funContainer.hpp"
#include <QList>
#include <QMap>

class SegmentList
{
public:
    void append(Segment seg);
    QMap<QString, QList<Segment> >& get();
    FunContainer digForFunctions() const;

    void store();

private:
    QMap<QString, QList<Segment> > _seg;
};

#endif
