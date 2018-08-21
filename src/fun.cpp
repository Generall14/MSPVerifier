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
    // Poniższa mapa zawiera listę punktów wejścia symulacji. Każdy przebieg symulacji wykonywany będzie od numeru lini i z rdzeniem
    // wskazanym w mapie.
    QMultiMap<int, Core> todo; // Lista symulacji (numer pierwszej instrukcji do wykonania, rdzeń wejścia)
    todo.insert(0, Core(_name)); // Punkt wejścia

    while(!todo.isEmpty())
    {
        // Pobieranie danych wejściowych symulacji i usuwanie z mapy wpisu. Obiekt prev zawsze będzie reprezentował stan rdzenia
        // poprzedający wykonanie instrukcji.
        auto it = todo.begin();
        Core* prev = new Core(it.value());
        int line = it.key();
        todo.erase(it);

        // Dla wszystkich lini od punktu wejścia:
        for(;line<_lines.size();line++)
        {
            // Tekst informacyjny jest pomijany:
            if(_lines.at(line).currentText.startsWith(" ;##fun", Qt::CaseInsensitive))
                continue;

            //================================================================================================
            // Wykrywanie powrotow: <TODO>
            if(Core::rets.contains(_lines.at(line).getInstruction()))
            {
                // <TODO> kwestia returnow
                break;
            }

            //================================================================================================
            // Wykrywanie skoków bezwarunkowych:
            // Dodawany do mapy wejść jest punkt wskazany przez etykietę jmp, aktualna symulacja jest kończona.
            if(Core::jumps.contains(_lines.at(line).getInstruction()))
            {
                if(_lines.at(line).core==nullptr)
                {
                    _lines[line].core = prev;
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

                if(_lines.at(line).getArguments().isEmpty())
                    throw std::runtime_error("Fun::simulate: brak etykiety skoku w "+_lines.at(line).toString().toStdString());
                // Wyszukiwanie etyiety:
                int found = -1; // <TODO> do refaktoryzacji
                for(int i=0;i<_lines.size();i++)
                {
                    if(_lines.at(i).getLabel()==_lines.at(line).getArguments().at(0))
                    {
                        found = i;
                        break;
                    }
                }
                if(found<0)
                    throw std::runtime_error("Fun::simulate: nie odnaleziono etykiety: "+_lines.at(line).toString().toStdString());
                todo.insert(found, Core(*prev));
                break;
            }

            //================================================================================================
            // Wykrywanie skoków warunkowych:
            // Dodawany do mapy wejść jest punkt wskazany przez etykietę jmp, aktualna symulacja jest kontynuowana.
            // W efekcie przyjmuje się rozgałęzienie symulacji na skocz lub nie skocz, bez sprawdzania warunków skoku.
            if(Core::jumpsIf.contains(_lines.at(line).getInstruction()))
            {

                if(_lines.at(line).getArguments().isEmpty())
                    throw std::runtime_error("Fun::simulate: brak etykiety skoku w "+_lines.at(line).toString().toStdString());
                // Wyszukiwanie etyiety:
                int found = -1; // <TODO> do refaktoryzacji
                for(int i=0;i<_lines.size();i++)
                {
                    if(_lines.at(i).getLabel()==_lines.at(line).getArguments().at(0))
                    {
                        found = i;
                        break;
                    }
                }
                if(found<0)
                    throw std::runtime_error("Fun::simulate: nie odnaleziono etykiety: "+_lines.at(line).toString().toStdString());
                todo.insert(found, Core(*prev));
            }

            //<TODO> - przypadki szczególne - call, jump...

            //================================================================================================
            // Ładowanie instrukcji nie wpływających na przepływ sterowania. Do rdzenia jest ładowana instrukcja, jeżeli dana linia
            // nie posiadała wcześniej symulowanego rdzenia - dostanie obiekt prev. Jeżeli linia miała juz własny rdzeń to zostanie
            // on połączony z prev, jeżeli to połączenie nie zmieni stanu rdzenia na bardziej nieokreślony (dalsza symulacja tego
            // przypadku niczego nie zmieni) - dany przebieg symulacji zostanie zakończony.
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
        prev = nullptr;
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
