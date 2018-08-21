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
    int line;
    // Lista zawiera punkty wyjścia (po wykrycie instrukcji ret kopia rdzenia zostanie dodana do tej listy)
    QList<Core> retStates;

    try
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
            line = it.key();
            todo.erase(it);

            // Dla wszystkich lini od punktu wejścia:
            for(;line<=_lines.size();line++)
            {
                // Sprawdzanie czy symulacja wyszła poza zakres kodu (co oznacza możliwość zakończenia kodu
                // bez instrukcji ret)
                if(line==_lines.size())
                {
                    Logger::LogError("Możliwość zakończenia kodu funkcji \'"+_name+"\" bez instrukcji powrotu");
                    continue;
                }

                // Tekst informacyjny jest pomijany:
                if(_lines.at(line).currentText.startsWith(" ;##fun", Qt::CaseInsensitive))
                    continue;

                //================================================================================================
                // Wykrywanie powrotow: <TODO>
                if(Core::rets.contains(_lines.at(line).getInstruction()))
                {
                    prev->loadInstruction(_lines.at(line));
                    if(_lines.at(line).core==nullptr)
                        _lines[line].core=prev;
                    else
                    {
                        _lines[line].core->merge(*prev);
                        delete prev;
                    }
                    prev = nullptr;
                    retStates.append(Core(*(_lines[line].core)));
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

                //<TODO> - przypadki szczególne - call, ...

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
            }
            delete prev;
            prev = nullptr;
        }

    _state = error; //<TODO> w piach, do testów
    }
    catch(std::runtime_error err)
    {
        Logger::LogError("Błąd krytyczny w funkcji \""+_name+"\", linia "+_lines.at(line).toSString()+": "+err.what());
        _state = error;
        return;
    }

    // Sprawdzanie stanu zwracanych rdzeni, w prawidłowej sytuacji wszystkie stosy powinny być na tym samym poziomie,
    // jeżeli tak nie jest - funkcja przyjmuje stan status error. Wszystkie zwrócone rdzenie są mergowane do
    // pojedynczego najogólniejszego który zostanie następnie sprawdzony pod względem zgodności z deklarowaną
    // konwencją funkcji.
    if(retStates.isEmpty())
    {
        _state = error;
        Logger::LogError("Brak punktu wyjścia w funkcji \""+_name+"\"");
        return;
    }
    else
    {
        Core ret(retStates.at(0));
        try
        {
            for(int i=1;i<retStates.size();i++)
                ret.merge(retStates.at(i));
        }
        catch(std::runtime_error err)
        {
            _state = error;
            QString out = "Niezgodne stany stosów przy wyjściu z funkcji \""+_name+"\":\n";
            for(auto ret: retStates)
                out.append(ret.toString());
            Logger::LogError(out);
            return;
        }
        // <TODO> sprawdź czy stan zwracanego stosu jest zgodny z deklaracjami
        // <TODO> zwracać faktyczny stan wyjścia czy deklarowany?
    }

    // <TODO>
    Logger::WriteFile("code/"+_name+".csv", toString());
}
