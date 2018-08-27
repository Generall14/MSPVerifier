#ifndef CONV_HPP
#define CONV_HPP

/**
  * @class Conv
  * @author Wojciech Kogut
  *
  * Klasa reprezentuje konwencję wywołań funkcji. Dla każdego rejestru zawiera typ obostrzeń jego wykorzystania według Conv::type, opcjonalnie
  * zaiera również maksymalny poziom pogłębienia stosu.
  */

#include <QMap>

class Fun;
class Reg;
class Stack;

class Conv
{
    friend class Convs;
public:
    bool checkFun(const Fun& fun, const Stack& stack) const;
    QString name() const;
    QString toString() const;
    QMap<QString, Reg> prepareReturnedRegs() const;

private:
    void set(QString reg, QString type, QString size);
    Conv(QString name);

    /**
     * Opisuje ograniczenia wykorzystania danego rejestru.
     */
    enum type
    {
        dontTouch, /**<Zabrania dotykania tego rejestru.*/
        touchAndRestore, /**<Pozwala na wykorzystanie rejestru pod warunkiem zachowania jego wartości w podanycm zakresie.*/
        touch /**<Pozwala na dowolne wykorzystanie rejestru.*/
    };

    /**
     * Grupuje typ ograniczenia i rozmiar.
     */
    struct typeS
    {
        type _type=dontTouch;
        QString _size="w";
    };
    Reg prepare(const typeS& typ, QString reg) const;

    QString _name = "?????"; /**<Nazwa konwencji.*/
    QMap<QString, typeS> _types; /**<Mapa rejestrów i ich ograniczeń.*/
    int _maxStack; /**<Maksymalny poziom powiększenia stosu przez funkcję.*/
};

#endif
