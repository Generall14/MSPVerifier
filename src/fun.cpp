#include "fun.hpp"
#include "Logger.hpp"
#include <stdexcept>
#include "funContainer.hpp"
#include <QMultiMap>
#include "core.hpp"
#include <QDebug>

Fun::Fun(QList<Line> lines)
{
    _lines = lines;

    if(lines.isEmpty())
        throw std::runtime_error("Fun::Fun: pusta lista lini kodu.");
    if(lines.size()<3)
        throw std::runtime_error("Fun::Fun: za malo lini kodu aby to mialo sens.");

    parse();

    Logger::WriteFile("code/"+_name+".csv", toString()); // <TODO> przerobić, status symulacji.
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
    QMultiMap<int, Core> todo; //Lista symulacji (numer pierwszej instrukcji do wykonania, rdzeń wejścia)
    todo.insert(0, Core(_name));

    while(!todo.isEmpty())
    {
        auto it = todo.begin();
        Core* prev = new Core(it.value());
        int line = it.key();
        todo.erase(it);

        for(;line<_lines.size();line++)
        {
            //<TODO> - przypadki szczególne - call, jump...

            prev->loadInstruction(_lines.at(line));
            if(_lines.at(line).core==nullptr)
            {
                _lines[line].core=prev;
                prev = new Core(*prev);
            }
            else
            {
                bool ret = _lines[line].core->merge(*prev);
                delete prev;
                prev = nullptr;
                if(ret)
                    prev = new Core(*_lines.at(line).core);
                else
                    break;
            }

            //<TODO> tutaj rodzie się kwestia poprzedniego rdzenia: poprzedni->ładuj instrukcje, merguj do
            // aktualnego/stworz aktualny. Poprzedni może być z polecenia w mapie lub z poprzedniej instrukcji.
            // jumpy warunkowe: dodaj do kolejki skok, symuluj dalej.
            // jumpy bezwarunkowe: dodaj do kolejki skok, zakończ pętle.
            //core.loadInstruction(_lines.at(line));
//            if()
//            _lines[line].core = new Core(core);
        }
        delete prev;
    }

    _state = error; //<TODO> w piach, do testów

//    for(auto line: _lines)
//    {
//        QString args;
//        for(auto arg: line.getArguments())
//            args.append(arg+", ");
//        args = args.mid(0, args.size()-2);
//        Logger::Log(line.currentText+" -> \""+line.getLabel()+"\" -> \""+line.getInstruction()+
//                    "."+line.getInstructionSize()+"\" -> \""+args+"\"");
//    } <TODO> w piach

    // <TODO>
    Logger::WriteFile("code/"+_name+".csv", toString());
}
