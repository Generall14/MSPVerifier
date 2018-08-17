#ifndef STACK_HPP
#define STACK_HPP

#include <QString>
#include <QStack>
#include <QMultiMap>
#include "fun.hpp"

class Stack
{
public:
    Stack(){}
    Stack(QString base);
    Stack(const Fun& getMaxStack);
    ~Stack();

    bool isEmpty() const;
    void call(const Fun& other, int size);
    void callUnknown(QString fun, int size);
    void ret();
    void pushB(QString desc);
    QString popB();

    int depth() const;

private:
    QStack<QString> _stack;
    QString _base = "";
    Fun* _called = nullptr;
    QString _ucalled = "";
    QMultiMap<QString, Stack> _unknowns;
    int lastCallS = 0;
};

#endif
