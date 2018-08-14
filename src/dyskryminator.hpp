#ifndef DYSKRYMINATOR_HPP
#define DYSKRYMINATOR_HPP

#include "dyskryminatorSM.hpp"

class Dyskryminator
{
public:
    Dyskryminator();

    void ResetState();
    void DyscriminateLine(QString &line, bool *wasComment = nullptr);

private:
    DyskryminatorSM stateMachine;
};

#endif
