#include "fun.hpp"
#include "Logger.hpp"
#include <stdexcept>
#include "funContainer.hpp"

Fun::Fun(QList<Line> lines)
{
    _lines = lines;

    if(lines.isEmpty())
        throw std::runtime_error("Fun::Fun: pusta lista lini kodu.");
    if(lines.size()<3)
        throw std::runtime_error("Fun::Fun: za malo lini kodu aby to mialo sens.");

    parse();

    Logger::WriteFile("code/"+_name+".txt", toString()); // <TODO> przerobić, status symulacji.
}

void Fun::parse()
{
    if(!_lines.at(1).currentText.contains(": ", Qt::CaseInsensitive))
        throw std::runtime_error("Fun::parse: brak etykiety w pierwszej lini kodu sekcji \""+
                                 _lines.at(1).currentText.toStdString()+"\" w pliku \""+
                                 _lines.at(0).File().toStdString()+"\".");

    _name = _lines.at(1).currentText.split(": ", QString::SkipEmptyParts).at(0);
    _name.remove(" ");
    //<TODO>
}

QString Fun::toString() const
{
    return LineContainer::toString();
    //<TODO>
}

Fun::simState Fun::state() const
{
    return _state;
}

QString Fun::name() const
{
    return _name;
}

void Fun::simulate(const FunContainer *fc)
{
    _state = error;
    // <TODO>
}
