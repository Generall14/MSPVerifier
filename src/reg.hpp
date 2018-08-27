#ifndef REG_HPP
#define REG_HPP

/**
  * @class Reg
  * @author Wojciech Kogut
  *
  * Klasa reprezentuje pojedynczy rejestr procesora, zachowuje wartości dla zakresów a, w, b. Posiada znacznik mówiący, że dany rejestr
  * był ruszany w trakcie symulacji. Posiada metodę ładującą nową wartość rejestru i mówiącą czy ta operacja zwiększyła entropię rejestru.
  */

#include <QString>
#include <QStringList>

class Stack;

class Reg
{
    friend class Core;
    friend class Conv;
public:
    Reg(){}
    Reg(QString val, QString size="w");

    QString toString() const;
    bool wasTouched() const;
    bool merge(const Reg& other);
    void push(Stack& stack, QString size="w");
    void pop(Stack& stack, QString size="w");

    static const QStringList regs; /**<Lista nazw rejestrów.*/

private:
    QString _a="?", _w="?", _b="?";
    bool _touched = false; /**<Znacznik mówiący czy rejestr był dotykany w trakcie symulacji..*/
};

#endif
