#include "stack.hpp"

Stack::Stack(QString base):
    _base(base)
{

}

Stack::Stack(const Fun& getMaxStack)
{
    //<TODO>
}

Stack::Stack(const Stack& other):
    _stack(other._stack),
    _base(other._base),
    _ucalled(other._ucalled),
    _unknowns(other._unknowns),
    lastCallS(other.lastCallS)
{
    if(other._called==nullptr)
        _called = nullptr;
    else
        _called = new Fun(*other._called);
}

Stack& Stack::operator=(const Stack& other)
{
    if(this==&other)
        return *this;

    delete _called;
    if(other._called==nullptr)
        _called = nullptr;
    else
        _called = new Fun(*other._called);

    _stack=other._stack;
    _base=other._base;
    _ucalled=other._ucalled;
    _unknowns=other._unknowns;
    lastCallS=other.lastCallS;

    return *this;
}

Stack::~Stack()
{
    delete _called;
}

bool Stack::isEmpty() const
{
    if(_base.isEmpty())
        return true;
    return false;
}

void Stack::call(const Fun& other, int size)
{
    if(_called!=nullptr)
        throw std::runtime_error("Stack::call: cos tu sie posralo");
    if(!_ucalled.isEmpty())
        throw std::runtime_error("Stack::call: cos tu sie posralo 2");
    _called = new Fun(other);

    while(size--)
        _stack.push("call");
}

void Stack::callUnknown(QString fun, int size)
{
    if(_called!=nullptr)
        throw std::runtime_error("Stack::callUnknown: cos tu sie posralo");
    if(!_ucalled.isEmpty())
        throw std::runtime_error("Stack::callUnknown: cos tu sie posralo 2");
    _ucalled = fun;
    lastCallS = size;
    while(size--)
        _stack.push("call");
}

void Stack::ret()
{
    while(lastCallS--)
        _stack.pop();
    // <TODO> zdejmij ze stosu zgodnie z poziomem powrotu funkcji
    delete _called;
    _called = nullptr;
    _ucalled = "";
}

void Stack::pushB(QString desc)
{
    _stack.push(desc);
}

QString Stack::popB()
{
    return _stack.pop();
}

int Stack::depth() const
{
    int dep = _stack.size();
//    if(_called!=nullptr)
//        dep += _called->depth(); <TODO> max glebokosc
    return dep;
}

QString Stack::toString() const
{
    return "Stack level: "+QString::number(_stack.size()); //<TODO> uwzględnić nieznane call?
}
