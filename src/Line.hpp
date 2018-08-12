#ifndef LINE_HPP
#define LINE_HPP

#include <QString>

//<TODO>
class Line
{
public:
    Line(QString file, QString text, int line);
    Line(Line& old, QString newLine="");

    QString File() const;
    QString OryginalText() const;
    int LineNumber() const;

    QString toString() const;

    QString currentText;

private:
    int _line;
    QString _oText;
    QString _file;
};

#endif
