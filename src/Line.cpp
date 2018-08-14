#include "Line.hpp"

Line::Line(QString file, QString text, int line):
    currentText(text),
    _line(line),
    _oText(text),
    _file(file)
{

}

Line::Line(const Line& old, QString newLine):
    _line(old.LineNumber()),
    _oText(old.OryginalText()),
    _file(old.File())
{
    currentText = newLine;
}

Line::Line(const Line& other):
    currentText(other.currentText),
    _line(other.LineNumber()),
    _oText(other.OryginalText()),
    _file(other.File())
{

}

QString Line::File() const
{
    return _file;
}

QString Line::OryginalText() const
{
    return _oText;
}

int Line::LineNumber() const
{
    return _line;
}

QString Line::toString() const
{
    QString temp = "[File: \""+_file+"\"";
    temp += ", line " + QString::number(_line);
    temp += ", text: \""+_oText;
    if(_oText!=currentText)
        temp += "\" -> \""+currentText;
    temp += "\"]";
    return temp;
}

QString Line::toSString() const
{
    QString temp = QString::number(_line)+": "+currentText;
    return temp;
}
