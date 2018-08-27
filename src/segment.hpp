#ifndef SEGMENT_HPP
#define SEGMENT_HPP

/**
  * @class Segment
  * @author Wojciech Kogut
  *
  * Klasa reprezentuje część kodu podzieloną na segmenty (według rseg/aseg).
  */

#include "LineContainer.hpp"
#include <QList>

class Line;

class Segment : public LineContainer
{
public:
    Segment(QList<Line> lines, QString type);

    QString type() const;

private:
    QString _type; /**<Typ segmentu.*/
};

#endif
