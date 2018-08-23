#ifndef REG_HPP
#define REG_HPP

#include <QString>
#include <QStringList>

class Stack;

class Reg
{
    friend class Core;
public:
    Reg(){}
    Reg(QString val, QString size="w");

    QString toString() const;
    bool wasTouched() const;
    bool merge(const Reg& other);
    void push(Stack& stack, QString size="w");
    void pop(Stack& stack, QString size="w");

    static const QStringList regs;

private:
    QString _a="?", _w="?", _b="?";
    bool _touched = false;
};

#endif
