#include "dyskryminator.hpp"

Dyskryminator::Dyskryminator()
{

}

void Dyskryminator::ResetState()
{
    stateMachine.ResetState();
}

void Dyskryminator::DyscriminateLine(QString &line, bool *wasComment)
{
    stateMachine.outBuff.clear();
    stateMachine.wasComment = false;

    for(QChar sgn: line)
        stateMachine.PushChar(sgn);
    stateMachine.PushEndl();

    line = stateMachine.outBuff;
    if(wasComment)
        *wasComment = stateMachine.wasComment;
}
