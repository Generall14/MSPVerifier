#include "LineContainer.hpp"

LineContainer::LineContainer(QList<Line> lines):
    _lines(lines)
{

}

/**
 * Zwraca referencje do danych.
 */
QList<Line>& LineContainer::get()
{
    return _lines;
}

/**
 * Zwraca abiorczy opis wszystkich przechowywanych danych.
 */
QString LineContainer::toString() const
{
    QString temp;
    for(auto line: _lines)
        temp += line.toString()+"\n";
    return temp;
}

/**
 * Zwraca abiorczy opis wszystkich przechowywanych danych w formie skróconej.
 */
QString LineContainer::toSString() const
{
    QString temp;
    for(auto line: _lines)
        temp += line.toSString()+"\n";
    return temp;
}
