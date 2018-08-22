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
    Stack(const Stack& other);
    Stack& operator=(const Stack& other);
    ~Stack();

    bool isEmpty() const;
    void call(const Fun& other, int size);
    void ret();
    void pushB(QString desc);
    void pushRandomStuff(QString size="w");
    QString popB();
    void popRandomStuff(QString size="w");
    bool merge(const Stack& other);

    QString toSString() const;
    QString toString() const;
    int depth() const;

private:
    QStack<QString> _stack;
    QString _base = "";
    Fun* _called = nullptr;
    int lastCallS = 0;//<TODO> wtfit

    QString getLocalString() const;
};

#endif
