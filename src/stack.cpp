#include "stack.hpp"
#include "fun.hpp"

Stack::Stack(QString base):
    _base(base)
{

}

Stack::Stack(const Stack& other):
    _stack(other._stack),
    _base(other._base),
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

void Stack::call(const Fun& other)
{
    if(_called!=nullptr)
        throw std::runtime_error("Stack::call: cos tu sie posralo");
    _called = new Fun(other);
}

void Stack::ret()
{
    while(lastCallS--)
        _stack.pop();
    // <TODO> zdejmij ze stosu zgodnie z poziomem powrotu funkcji
    delete _called;
    _called = nullptr;
    // <TODO> wtfit?
}

void Stack::pushB(QString desc)
{
    _stack.push(desc);
}

QString Stack::popB()
{
    if(_stack.isEmpty())
        throw std::runtime_error("Stack::popB: stos jest pusty!");
    return _stack.pop();
}

int Stack::depth() const
{
    int dep = _stack.size();
    if(_called!=nullptr)
        dep += _called->getMaxStack().depth();
    return dep;
}

QString Stack::toSString() const
{
    return "Stack level: "+QString::number(this->depth());
}

QString Stack::toString() const
{
    QString temp = "Total depth: " + QString::number(this->depth());
    temp.append(": "+getLocalString());
    return temp;
}

QString Stack::getLocalString() const
{
    QString temp = _base+"("+QString::number(_stack.size())+")";
    if(_called!=nullptr)
        temp.append(" -> "+_called->getMaxStack().getLocalString());
    return temp;
}

/**
 * Merguje do siebie wskazany obiekt, jeżeli nastąpiło zwiększenie entropii zwrócona zostanie wartość true. Jeżeli
 * stosy okażą się niezgodne (inne poziomy) - zgłoszoy zostanie wyjątek.
 */
bool Stack::merge(const Stack& other)
{
    if(other.depth()!=this->depth())
        throw std::runtime_error("Stack::merge: próba mergowania stosów o różnej głębokości: \n"+
                                 this->toString().toStdString()+"\nvs\n"+
                                 other.toString().toStdString());

    bool endstate = false;
    for(int i=0;i<_stack.size();++i)
    {
        if(_stack.at(i)!=other._stack.at(1))
        {
            _stack[i] = "??";
            endstate = true;
        }
    }

    return endstate;
}

void Stack::pushRandomStuff(QString size)
{
    if(size=="b")
    {
        _stack.push("?");
    }
    else if(size=="w")
    {
        _stack.push("?");
        _stack.push("?");
    }
    else if(size=="a")
    {
        _stack.push("?");
        _stack.push("?");
        _stack.push("?");
        _stack.push("?");
    }
    else
        throw std::runtime_error("Stack::pushRandomStuff: nieprawidlowy specyfikator rozmiaru: \""+size.toStdString()+"\".");
}

void Stack::popRandomStuff(QString size)
{
    if(size=="b")
    {
        _stack.pop();
    }
    else if(size=="w")
    {
        _stack.pop();
        _stack.pop();
    }
    else if(size=="a")
    {
        _stack.pop();
        _stack.pop();
        _stack.pop();
        _stack.pop();
    }
    else
        throw std::runtime_error("Stack::popRandomStuff: nieprawidlowy specyfikator rozmiaru: \""+size.toStdString()+"\".");
}

Stack Stack::prepare(int depth, QString name)
{
    Stack temp;
    temp._base=name;
    while(depth--)
        temp.pushB("X");
    return temp;
}
