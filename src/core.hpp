#ifndef CORE_HPP
#define CORE_HPP

/**
  * @class Core
  * @author Wojciech Kogut
  *
  * Klasa reprezentuje stan symulowanego rdzenia MSP430 (zawartość rejestrów r4-r15 oraz stan stosu). Udostępnia metodę loadInstruction która
  * symuluje wykonanie pojedynczej instrukcji.
  */

#include <QString>
#include <QMap>
#include "reg.hpp"
#include "stack.hpp"

class Line;
class Fun;

class Core
{
    friend class Fun;
public:
    Core(QString name);
    Core(const Core& other);
    Core& operator=(const Core& other);

    bool merge(const Core& other);
    bool loadInstruction(const Line& line);
    void loadReturnedRegs(const QMap<QString, Reg>& regs);
    void call(const Fun& fun);
    int returns() const;

    QString toString() const;

    static const QStringList jumps; /**<Zbiór instrukcji realizujących skoki bezwarunkowe.*/
    static const QStringList jumpsIf; /**<Zbiór instrukcji realizujących skoki warunkowe.*/
    static const QStringList rets; /**<Zbiór instrukcji powrotu z funkcji.*/
    static const QStringList calls; /**<Zbiór instrukci wywoływania funkcji.*/

private:
    QString _name="";
    Stack _stack; /**<Stos w danym momencie symulacji.*/
    QMap<QString, Reg> _regs; /**<Mapa rejestrów.*/
    int _returns=0; /**<Poziom powrotu z funkcji.*/

    static const QStringList biMArgs; /**<Zbiór instrukcji dwuargumentowych modyfikujących drugi argument.*/
    static const QStringList singleMArgs; /**<Zbiór instrukcji jednoargumentowych modyfikujących argument.*/
    static const QStringList transparentArgs; /**<Zbiór instrukcji nie wpływających na stan rdzenia.*/
    static const QStringList pushes; /**<Zbiór instrukcji odkładających dane na stos.*/
    static const QStringList pops; /**<Zbiór instrukcji pobierających dane ze stosu.*/
    static const QStringList adds; /**<Zbiór instrukcji dodających wartości.*/
    static const QStringList subs; /**<Zbiórinstrukcji odejmujących wartości.*/
};

#endif
