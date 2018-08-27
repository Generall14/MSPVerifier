#ifndef DYSKRYMINATOR_HPP
#define DYSKRYMINATOR_HPP

/**
  * @class Dyskryminator
  * @author Wojciech Kogut
  *
  * Klasa służy do usuwania komentarzy z kodu asemblera, jest to podpieprzona i zuborzona wersja z projektu KWLiner.
  */

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
