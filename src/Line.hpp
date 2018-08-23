#ifndef LINE_HPP
#define LINE_HPP

#include <QString>
#include <QStringList>

class Core;

class Line
{
public:
    Line(QString file, QString text, int line);
    Line(const Line& old, QString newLine);
    Line(const Line& other);
    Line& operator=(const Line& other);
    ~Line();

    QString File() const;
    QString OryginalText() const;
    int LineNumber() const;

    QString toString() const;
    QString toSString() const;

    QString getInstruction() const;
    QString getInstructionSize() const;
    QString getLabel() const;
    QStringList getArguments() const;

    QString currentText;
    Core* core = nullptr; //Obiekt symulowanego rdzenia po wykonaniu instrukcji

private:
    int _line;
    QString _oText;
    QString _file;
};

#endif
