#include "fun.hpp"
#include "Logger.hpp"
#include <stdexcept>
#include "funContainer.hpp"
#include <QMultiMap>
#include "core.hpp"
#include "Line.hpp"
#include "convs.hpp"
#include <iostream>

/**
 * Konstruktor, przyjmuje linie kodu przetworzone i podzielone, pierwsza linia powinna zawierać instrukcję ;##fun(...).
 */
Fun::Fun(QList<Line> lines)
{
    _lines = lines;

    if(lines.isEmpty())
        throw std::runtime_error("Fun::Fun: pusta lista lini kodu.");
    if(lines.size()<3)
        throw std::runtime_error("Fun::Fun: za malo lini kodu aby to mialo sens.");

    parse();
}

/**
 * Odczytuje wymagane dane z zawartego tekstu.
 */
void Fun::parse()
{
    if(!_lines.at(0).currentText.startsWith(" ;##fun", Qt::CaseInsensitive))
        throw std::runtime_error("Fun::parse: brak znacznika funkcji na początku kodu sekcji w pliku \""+
                                 _lines.at(0).File().toStdString()+"\".");

    int start, end;
    start = _lines.at(0).currentText.indexOf("(");
    end = _lines.at(0).currentText.indexOf(")");
    if((start<0)||(end<0))
        throw std::runtime_error("Fun::parse: nie można odczytać typu konwencji sekcji \""+
                                 _lines.at(0).currentText.toStdString()+"\" w pliku \""+
                                 _lines.at(0).File().toStdString()+"\".");

    _convention = _lines.at(0).currentText.mid(start+1, end-start-1);

    if(!_lines.at(1).currentText.contains(": ", Qt::CaseInsensitive))
        throw std::runtime_error("Fun::parse: brak etykiety w pierwszej lini kodu sekcji \""+
                                 _lines.at(1).currentText.toStdString()+"\" w pliku \""+
                                 _lines.at(0).File().toStdString()+"\".");

    _name = "#"+_lines.at(1).currentText.split(": ", QString::SkipEmptyParts).at(0);
    _name.remove(" ");
}

/**
 * Zachowuje opis funkcji (raport z symulacji oraz kod).
 */
QString Fun::toString() const
{
    QString temp;
    temp += "Etykieta funkcji: \""+_name+"\"\n";
    temp += "Plik: \""+_lines.at(0).File()+"\"\n";

    temp += "Status symulacji: ";
    if(_state==waiting)
        temp += "waiting";
    else if(_state==done)
        temp += "done";
    else if(_state==error)
        temp += "error: "+_errorDesc;
    temp += "\n";

    temp += "Typ konwencji wywołań: "+_convention+", status: "+_convState+"\n";
    temp += "Maksymalny stos: " + this->getMaxStack().toString()+"\n";
    temp += "Powrót na poziomie: "+QString::number(_returns)+"\n";
    temp += "Zwracany stan rejestrów:\n";
    for(auto key: _retRegs.keys())
        temp += key+_retRegs[key].toString()+"\n";
    temp += "\n\n"+LineContainer::toString();
    return temp;
}

/**
 * Zwraca status symulacji.
 */
Fun::simState Fun::state() const
{
    return _state;
}

/**
 * Zwraca nazwę funkcji.
 */
QString Fun::name() const
{
    return _name;
}

/**
 * Wykonuje symulację, nie każde wywołanie funkcji zakończy się sukcesem, może się okazać że inne funkcje wołane instrukcją call nie będą
 * jeższe zasymulowane, w takim przypadku należy wywołać metodę simulate pozostałych funkcji a następnie wrócić do tej. Jeżeli funkcje
 * wołane będą posiadały status error, błęd ten będzie wleczony i obiekt this również przyjmie status error.
 *
 * Przy wywołaniach call pośrednich lub takich których nie ma we wskazanym FunContainer metoda spróbuje podmienić funkcję (przez repfun)
 * lub spreparować funkcję na podstawie
 * podanych wartości domyślnych (przez defconv, deffundepth, deffunret), jeżeli któraś z tych instrukcji nie wystąpi przed wywołaniem
 * call - funkcja przyjmie status error.
 * @param fc - Wskaźnik na obiekt z listą funkcji.
 * @param convs - Wskaźnik na obiekt zawierający opis zdefiniowanych konwencji.
 */
void Fun::simulate(const FunContainer *fc, const Convs *convs)
{
//    std::cout << _name.toStdString() << std::endl;

    // Symulacja krok po kroku linię kodu, poczynając od punktów wejścia zawartych w todo (zawiera numer lini i stan wejściowy rzenia).
    // Każda instrukcja wpływa odpowiednio na stan symulowanego rdzenia, każda symulacja jest wykonywana aż do napoitkania instrukcji
    // powrotu, końca funkcji, skoku bezwarunkowego lub jeżeli wykonanie danej instrukcji nie zwiększy entropii rdzenia w danej linii.

    // Symulacja jest powtarzana aż mapa todo się opróżni. Początkowo todo posiada jeden punkt wejścia (wejście funkcji), lecz w kolejnych
    // iteracjach symulacji mogą być dodawane kolejne wejścia po instrukcjach skoków.

    int line=0;
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

            QString defaultConvention = "";
            QString replaceFun = "";
            uint defaultFunDepth = 666;
            int defaultFunRet = 0;
            bool ignoreInstruction = false;

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

                if(ignoreInstruction)
                {
                    ignoreInstruction = false;
                    continue;
                }

                // Instrukcje specjalne, linia rozpoczęta ;##, instrukcje dzielone średnikiem:
                // ;##ins1(arg);ins2(arg)
                else if(_lines.at(line).currentText.startsWith(" ;##", Qt::CaseInsensitive))
                {
                    QStringList inss = _lines.at(line).currentText.mid(4).remove(" ").split(";");
                    for(QString insl: inss)
                    {
                        int start, end;
                        start = insl.indexOf("(");
                        end = insl.indexOf(")");

                        QString ins = insl.mid(0, start);
                        QString arg = "";
                        if((start>0)&&(end>0))
                            arg = insl.mid(start+1, end-start-1);
                        if(!ins.compare("fun", Qt::CaseInsensitive))
                            continue;
                        else if(!ins.compare("defconv", Qt::CaseInsensitive))
                            defaultConvention = arg;
                        else if(!ins.compare("deffundepth", Qt::CaseInsensitive))
                            defaultFunDepth = arg.toInt();
                        else if(!ins.compare("deffunret", Qt::CaseInsensitive))
                            defaultFunRet = arg.toInt();
                        else if(!ins.compare("ignorenextline", Qt::CaseInsensitive))
                            ignoreInstruction = true;
                        else if(!ins.compare("repfun", Qt::CaseInsensitive))
                            replaceFun = "#"+arg;
                        else
                            Logger::LogWarning("Nieznany argument w "+_lines.at(line).toString());
                    }
                    continue;
                }

                //================================================================================================
                // Wykrywanie powrotow: linia jest wykonywana, kopia rdzenia jest dodawana do listy retStates,
                // aktualna iteracja symulacji jest kończona. Po zakończeniu symulacji wszystkie elementy listy
                // są porównywane pod względem spójności poziomu wyjścia, zwracany stan rejestrów jest przyjmowany
                // na podstawie sumy (wszystkie możliwości zmian) z listy rdzeni po ret.
                else if(Core::rets.contains(_lines.at(line).getInstruction()))
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
                else if(Core::jumps.contains(_lines.at(line).getInstruction()))
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
                    if(_lines.at(line).getArguments().at(0)=="$")
                        break;
                    // Wyszukiwanie etykiety:
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
                else if(Core::jumpsIf.contains(_lines.at(line).getInstruction()))
                {
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

                //================================================================================================
                // Wykrywanie wołań funkcji. Funkcja wywoływana jest pobierana z listy funkcji, jeżeli jej tam nie
                // ma, spreparowana zostanie funkcja opisana przez polecenia ;## <TODO>, jeżeli takie nie wystąpią
                // przed daną linią symulacja zakończy się błędem. Do obiektu Core w instrukcji call zostanie
                // dopisana instrukcja call oraz maksymalny stos zwracany z wołanej funkcji (jest to wyjątkowa
                // sytuacja, wszystkie inne Core posiadają stan stosu po wykonaniu instrukcji). Rdzeń służący
                // do dalszej symulacji (prev) zostanie uzupełniony o zmiany w rejestrach spowodowane wywołaniem
                // funkcji.
                else if(Core::calls.contains(_lines.at(line).getInstruction()))
                {
                    if(_lines.at(line).core!=nullptr)
                        break;
                    if(_lines.at(line).getArguments().isEmpty())
                        throw std::runtime_error("Fun::simulate: brak etykiety funkcji w "+_lines.at(line).toString().toStdString());

                    // Pobieranie obiektu funkcji
                    Fun called;
                    try
                    {
                        if((!replaceFun.isEmpty()))
                        {
                            called = fc->getByName(replaceFun);
                            replaceFun.clear();
                        }
                        else
                            called = fc->getByName(_lines.at(line).getArguments().at(0));
                    }
                    catch(...)
                    {
                        try
                        {
                            called = Fun::prepareFake("nieznane wolanie w "+_lines[line].toSString(), defaultConvention,
                                                      defaultFunDepth, defaultFunRet, convs);
                        }
                        catch(std::runtime_error err)
                        {
                            throw std::runtime_error("brak funkcji "+_lines.at(line).getArguments().at(0).toStdString()+", "+err.what());
                        }
                    }

                    // Sprawdzanie jej stanu
                    if(called.state()==error)
                        throw std::runtime_error("Fun::simulate: nie można wykonać instrukcji call z powodu "
                                                 "wleczonego błędu funkcji wołanej");
                    // Jeżeli funkcja jeszcze nie została przesymulowana - przerwij własną symulacje.
                    else if(called.state()==waiting)
                    {
                        for(int d=0;d<_lines.size();d++)
                        {
                            delete _lines[d].core;
                            _lines[d].core = nullptr;
                        }
                        _state = waiting;
                        return;
                    }
                    // Jest ok - wykonaj isntrukcję, przypisz do Core linii maksymalny stos zwracany przez
                    // funkcję wołaną, sprawdź czy rozmiary instrukcji call i powrotu są zgone. Do prev przypisz
                    // zmiany rejestrów.
                    else
                    {
                        Core* cPrev = new Core(*prev);
                        cPrev->loadInstruction(_lines.at(line));
                        if(prev->_stack.depth()!=(cPrev->_stack.depth()+called.getReturnedLevel()))
                            throw std::runtime_error("Fun::simulate: niedopasowanie instrukcji wołania i powrotu");

                        cPrev->call(called);
                        _lines[line].core = cPrev;
                        prev->loadReturnedRegs(called.getReturnedRegs());
                    }
                }

                //================================================================================================
                // Ładowanie instrukcji nie wpływających na przepływ sterowania. Do rdzenia jest ładowana instrukcja, jeżeli dana linia
                // nie posiadała wcześniej symulowanego rdzenia - dostanie obiekt prev. Jeżeli linia miała juz własny rdzeń to zostanie
                // on połączony z prev, jeżeli to połączenie nie zmieni stanu rdzenia na bardziej nieokreślony (dalsza symulacja tego
                // przypadku niczego nie zmieni) - dany przebieg symulacji zostanie zakończony.
                else
                {
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
            }
            delete prev;
            prev = nullptr;
        }
    }
    catch(std::runtime_error err)
    {
        _errorDesc = "Błąd krytyczny w funkcji \""+_name+"\", linia "+_lines.at(line).toSString()+": "+err.what();
        Logger::LogError(_errorDesc);
        _state = error;
        Logger::WriteFile("code/"+_name.mid(1)+".csv", toString());
        return;
    }

    // Sprawdzanie stanu zwracanych rdzeni, w prawidłowej sytuacji wszystkie stosy powinny być na tym samym poziomie,
    // jeżeli tak nie jest - funkcja przyjmuje stan status error. Wszystkie zwrócone rdzenie są mergowane do
    // pojedynczego najogólniejszego który zostanie następnie sprawdzony pod względem zgodności z deklarowaną
    // konwencją funkcji.
    if(retStates.isEmpty())
    {
        _errorDesc = "Brak punktu wyjścia w funkcji \""+_name+"\"";
        _state = error;
        Logger::LogError(_errorDesc);
        Logger::WriteFile("code/"+_name.mid(1)+".csv", toString());
        return;
    }
    else
    {
        Core ret(retStates.at(0));
        try
        {
            for(int i=1;i<retStates.size();i++)
                ret.merge(retStates.at(i));
            _retRegs = ret._regs;
            _returns = ret.returns();
        }
        catch(std::runtime_error err)
        {
            _state = error;
            _errorDesc = "Niezgodne stany stosów przy wyjściu z funkcji \""+_name+"\":\n";
            for(auto ret: retStates)
                _errorDesc.append(ret.toString());
            Logger::LogError(_errorDesc);
            Logger::WriteFile("code/"+_name.mid(1)+".csv", toString());
            return;
        }
    }

    // Obliczanie maksymalnego poziomu stosu:
    for(auto line: _lines)
    {
        if(line.core==nullptr)
            continue;
        if(line.core->_stack.depth()>_maxStack.depth())
            _maxStack = line.core->_stack;
    }

    // Sprawdzanie zgodności z przyjętą konwencją.
    if(!convs->checkFun(*this, _maxStack))
        _convState = "ok";

    _state = done;
    Logger::WriteFile("code/"+_name.mid(1)+".csv", toString());
}

/**
 * Zwraca maksymalny poziom stosu jaki zajmuje funkcja (nie licząc adresu powrotu odłożonego przez call);
 */
Stack Fun::getMaxStack() const
{
    return _maxStack;
}

/**
 * Zwraca poziom powrotu z funkcji (-2 dla ret, -4 dla reta)
 */
int Fun::getReturnedLevel() const
{
    return _returns;
}

/**
 * Zwraca stan zwracanych rejestrów (wszystkie możliwe zmiany jekie mogą wynikac z wywołania tej funcji).
 */
QMap<QString, Reg> Fun::getReturnedRegs() const
{
    return _retRegs;
}

/**
 * Zwraca nazwę przyjętej konwencji wywołań.
 */
QString Fun::getConventionType() const
{
    return _convention;
}

/**
 * Tworzy spreparowany obiekt funkcji na podstawie podanych parametrów.
 * @param name - Nazwa funkcji.
 * @param conv - Typ konwencji.
 * @param depth - Maksymalna głębokość zajmowanego stosu.
 * @param ret - Poziom powrotu z funckji.
 * @param convs - Obiekt listy konwencji.
 */
Fun Fun::prepareFake(QString name, QString conv, uint depth, int ret, const Convs* convs)
{
    Fun temp;
    temp._name = name;
    temp._state = done;
    temp._retRegs = convs->prepareReturnedRegs(conv);
    temp._returns = ret;
    temp._convention = conv;
    temp._maxStack = Stack::prepare(depth, name);
    return temp;
}
