#include "segmentList.hpp"
#include "Logger.hpp"

void SegmentList::append(Segment seg)
{
    _seg[seg.type()].append(seg);
}

QMap<QString, QList<Segment> > &SegmentList::get()
{
    return _seg;
}

void SegmentList::store()
{
    for(QString key: _seg.keys())
    {
        QString temp;
        for(auto seg: _seg[key])
        {
            temp.append(seg.toString());
            temp.append("\n\n\n\n\n\n");
        }

        Logger::WriteFile("sections/"+key+".txt", temp);
    }
}
