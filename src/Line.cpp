#include "Line.hpp"
#include <QDebug>
#include "core.hpp"

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
    if(other.core==nullptr)
        core = nullptr;
    else
        core = new Core(*other.core);
}

Line& Line::operator=(const Line& other)
{
    if(this==&other)
        return *this;

    delete core;
    if(other.core==nullptr)
        core = nullptr;
    else
        core = new Core(*other.core);

    currentText=other.currentText;
    _line=other.LineNumber();
    _oText=other.OryginalText();
    _file=other.File();

    return *this;
}

Line::~Line()
{
    delete core;
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
    QString temp = "File:'\""+_file+"\"";
    temp += "'line:'" + QString::number(_line);
    temp += "'text:'\""+_oText;
    if(_oText!=currentText)
        temp += "\"'->'\""+currentText;
    temp += "\"'core:'";
    if(core==nullptr)
        temp += "nullptr'";
    else
        temp += core->toString();
    return temp;
}

QString Line::toSString() const
{
    QString temp = QString::number(_line)+": \""+currentText+"\"";
    return temp;
}

QString Line::getInstruction() const
{
    if(currentText.startsWith(" ;##", Qt::CaseInsensitive))
        return "";
    QString temp = currentText;
    if(temp.contains(":"))
        temp = temp.split(":").at(1);
    QStringList sl = temp.split(" ", QString::SkipEmptyParts);
    if(sl.isEmpty())
        return "";
    else
        return sl.at(0).split(".").at(0);
}

QString Line::getInstructionSize() const
{
    if(currentText.startsWith(" ;##", Qt::CaseInsensitive))
        return "";
    QString temp = currentText;
    if(temp.contains(":"))
        temp = temp.split(":").at(1);
    QStringList sl = temp.split(" ", QString::SkipEmptyParts);
    if(sl.isEmpty())
        return "";
    else
    {
        sl = sl.at(0).split(".", QString::SkipEmptyParts);
        if(sl.size()<2)
            return "w";
        else
            return sl.at(1);
    }
}

QString Line::getLabel() const
{
    if(currentText.startsWith(" ;##", Qt::CaseInsensitive))
        return "";
    QString temp;
    if(currentText.contains(":"))
    {
        temp = currentText.split(":").at(0);
        temp.remove(" ");
    }
    return temp;
}

QStringList Line::getArguments() const
{
    if(currentText.startsWith(" ;##", Qt::CaseInsensitive))
        return QStringList();
    QString temp = currentText;
    if(temp.contains(":"))
        temp = temp.split(":").at(1);
    QStringList sl = temp.split(" ", QString::SkipEmptyParts);
    if(sl.size()<2)
        return QStringList();
    else
    {
        sl = sl.mid(1);
        for(QString& a: sl)
            a.remove(",");
        return sl;
    }
}
