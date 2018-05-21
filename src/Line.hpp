#ifndef LINE_HPP
#define LINE_HPP

#include <QString>

//<TODO>
class Line
{
public:
    Line(QString file, QString text, int line);

    QString File() const;
    QString TextOriginal() const;
    int LineInFile() const;

    QString toString() const;

private:
    int _line;
    QString _text;
    QString _file;
};

#endif
