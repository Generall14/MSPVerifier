#ifndef SEGMENTLIST_HPP
#define SEGMENTLIST_HPP

/**
  * @class SegmentList
  * @author Wojciech Kogut
  *
  * Klasa zawiera zbiór segmentów poszregowany według typu segmentu. Posiada metodę która z segmentów o typie code wyciąga części podzielone
  * jako funkcje i zwraca je jako FunContainer.
  *
  * Przechowywanie segmentów innych niż code nie ma w tym momencie sensu, jest to pozostałość z dawnych czasów gdy projekt ten miał
  * szersze, wręcz imperialistyczne aspiracje.
  */

#include "segment.hpp"
#include <QList>
#include <QMap>

class FunContainer;

class SegmentList
{
public:
    void append(Segment seg);
    QMap<QString, QList<Segment> >& get();
    FunContainer digForFunctions(bool quiet) const;

    void store();

private:
    QMap<QString, QList<Segment> > _seg;
};

#endif
