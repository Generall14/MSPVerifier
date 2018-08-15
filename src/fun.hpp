#ifndef FUN_HPP
#define FUN_HPP

#include "LineContainer.hpp"
#include <QList>
#include "Line.hpp"

class Fun : public LineContainer
{
public:
    Fun(QList<Line> lines);

    QString toString() const;

private:
    void parse();

    QString _name="__XXXXXX_tu_sie_cos_zesralo";
};

#endif
