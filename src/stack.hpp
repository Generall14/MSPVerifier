#ifndef STACK_HPP
#define STACK_HPP

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
    QStack<QString> _stack;
    QString _base = "";
    Fun* _called = nullptr;

    QString getLocalString() const;
};

#endif
