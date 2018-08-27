#ifndef STACK_HPP
#define STACK_HPP

/**
  * @class Stack
  * @author Wojciech Kogut
  *
  * Klasa reprezentuje stos pojedynczej funkcji. Posiada wskaźnik do wywoływanej funkcji którego odczyt daje pełen obraz stosu.
  */

#include <QString>
#include <QStack>
#include <QMultiMap>

class Fun;

class Stack
{
public:
    Stack(){}
    Stack(QString base);
    Stack(const Stack& other);
    Stack& operator=(const Stack& other);
    ~Stack();

    bool isEmpty() const;
    void call(const Fun& other);
    void pushB(QString desc);
    void pushRandomStuff(QString size="w");
    QString popB();
    void popRandomStuff(QString size="w");
    bool merge(const Stack& other);

    QString toSString() const;
    QString toString() const;
    int depth() const;

    static Stack prepare(int depth, QString name);

private:
    QStack<QString> _stack; /**<Reprezentuje dane lokalnego stosu.*/
    QString _base = ""; /**<Nazwa funkcji bazowej względem liczony jest stos.*/
    Fun* _called = nullptr; /**<Wskaźnik na funkcję wywołaną w danym momencie, rekurencyjne wywołanie this->_called->getMaxStack()._called...
                                daje pełną historię wywołań.*/

    QString getLocalString() const;
};

#endif
