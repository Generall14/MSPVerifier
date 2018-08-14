#include "segmentList.hpp"
#include "Logger.hpp"

void SegmentList::append(Segment seg)
{
    _seg[seg.type()].append(seg);

    if(seg.type()=="unknown")
        Logger::LogWarning("Nie rozpoznano sekcji w pliku \""+seg.get().at(0).File()+"\". Patrz sections/unknown.txt.");
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
