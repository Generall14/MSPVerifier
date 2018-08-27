#ifndef FUN_HPP
#define FUN_HPP

/**
  * @class Fun
  * @author Wojciech Kogut
  *
  * Klasa reprezentuje pojedynczą funkcję, posiada metodę symulującą jej działanie. Jej stan opisuje metoda Fun::state(). Po zakończeniu
  * symulacji może zwrócić informacje o funkcji takie jak maksymalny zajmowany stos, poziom powrotu, zwracany stan modyfikowanych
  * rejestrów, typ konwencji wywołań.
  */

#include "LineContainer.hpp"
#include <QList>
#include <QMap>
#include "reg.hpp"
#include "stack.hpp"

class FunContainer;
class Line;
class Convs;

class Fun : public LineContainer
{
public:
    Fun(){}
    Fun(QList<Line> lines);

    void simulate(const FunContainer* fc, const Convs* convs);

    /**
     * Określa status symulacji.
     */
    enum simState
    {
        waiting, /**<Funkcja oczekuje na symulacje (jeszcze nie rozpoczęta lub nie mogła być dokończona z powodu nie ukończonej
                    symulacji funkcji wywoływanej).*/
        done, /**<Smulacja zakończona.*/
        error /**<Symulacja nie mogła być zakończona z powodu błędów.*/
    };

    QString toString() const;
    simState state() const;
    QString name() const;
    Stack getMaxStack() const;
    int getReturnedLevel() const;
    QMap<QString, Reg> getReturnedRegs() const;
    QString getConventionType() const;

    static Fun prepareFake(QString name, QString conv, uint depth, int ret, const Convs* convs);

private:
    void parse();

    QString _name="__XXXXXX_tu_sie_cos_zesralo"; /**<Nazwa funkcji.*/
    simState _state=waiting; /**<Statuc symulacji.*/
    QString _errorDesc; /**<Opis błędu symulacji (jeżeli wystąpił).*/
    QMap<QString, Reg> _retRegs; /**<Stan zwracanych rejestrów.*/
    int _returns=-666; /**<Poziom powrotu (-2 dla instrukcji ret, -4 dla instrukcji reta).*/
    QString _convention="???"; /**<Nazwa konwencji przyjętej dla funkcji.*/
    QString _convState = "ERROR"; /**<Status spełnienia warunków konwencji.*/
    Stack _maxStack; /**<Maksymalny zwracany stos.*/
};

#endif
