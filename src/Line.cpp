#include "Line.hpp"
#include "core.hpp"
#include <qdebug.h>

/**
 * Konstruktor przyjmujący pierwotne wartości danych.
 * @param file - Nazwa pliku z którego pochodzą dane.
 * @param text - Tekst.
 * @param line - Numer lini w pliku.
 */
Line::Line(QString file, QString text, int line):
    currentText(text),
    _line(line),
    _oText(text),
    _file(file)
{

}

/**
 * Konstruktor kopiujący który dodatkowo nadpisuje wartość currentText.
 * @param newLine - Ten tekst będzie wpisany do currentText.
 */
Line::Line(const Line& old, QString newLine):
    _line(old.LineNumber()),
    _oText(old.OryginalText()),
    _file(old.File())
{
    currentText = newLine;
}

/**
 * Konstruktor kopiujący.
 */
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

/**
 * Zwraca nazwę pliku z którego pochodzi dany tekst.
 */
QString Line::File() const
{
    return _file;
}

/**
 * Zwraca pierwotny tekst (taki jaki został podany w konstruktorze, czyli w założeniu taki jaki znajduje się w pliku źródłowym).
 */
QString Line::OryginalText() const
{
    return _oText;
}

/**
 * Zwraca numer lini w pliku źródłowym z której pochodzi dana linia.
 */
int Line::LineNumber() const
{
    return _line;
}

/**
 * Zwraca pełen opis obiektu.
 */
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

/**
 * Zwraca skrócony opis obiektu.
 */
QString Line::toSString() const
{
    QString temp = QString::number(_line)+": \""+currentText+"\"";
    return temp;
}

/**
 * Zwraca wyciągniętą instrukcje asemblera z danej lini, błędne odczytanie lub brak instrukcji spowoduje zwrócenie pustego ciągu. Metoda ta
 * gwarantuje poprawny wynik tylko jeżeli linia przeszła wszystkie operacje przetwarzania w obiekcie Fun.
 */
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

/**
 * Metoda zwraca rozmiar instrukcji (a/w/b), jeżeli w kodzie nie jest określona zwrócony zostanie rozmiar w. Metoda ta gwarantuje poprawny
 * wynik tylko jeżeli linia przeszła wszystkie operacje przetwarzania w obiekcie Fun.
 */
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

/**
 * Zwraca etykiete instrukcji, jeżeli w kodzie etykieta nie występuje zwrócie pusty ciąg. Metoda ta gwarantuje poprawny wynik tylko
 * jeżeli linia przeszła wszystkie operacje przetwarzania w obiekcie Fun.
 */
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

/**
 * Zwraca listę argumentów instrukcji, jeżeli nie odnajdzie argumentów zwrócona zostanie pusta lista. Metoda ta gwarantuje poprawny
 * wynik tylko jeżeli linia przeszła wszystkie operacje przetwarzania w obiekcie Fun.
 */
QStringList Line::getArguments() const
{
    if(currentText.startsWith(" ;##", Qt::CaseInsensitive))
        return QStringList();
    QString temp = currentText;
    if(temp.contains(":"))
        temp = temp.split(":").at(1);
    while(temp.startsWith(" "))
        temp.remove(0, 1);
    int found = temp.indexOf(" ");
    if((found<0)||(found==temp.size()-1))
        return QStringList();
    temp = temp.mid(found);
    temp = temp.remove(" ");
    return temp.split(",");
}
