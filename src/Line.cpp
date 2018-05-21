#include "Line.hpp"

Line::Line(QString file, QString text, int line):
    _line(line),
    _text(text),
    _file(file)
{

}

QString Line::File() const
{
    return _file;
}

QString Line::TextOriginal() const
{
    return _text;
}

int Line::LineInFile() const
{
    return _line;
}

QString Line::toString() const
{
    QString temp = QString::number(_line)+" "+_text;
    return temp;
}
