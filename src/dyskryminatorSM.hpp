#ifndef DYSKRYMINATORSM_HPP
#define DYSKRYMINATORSM_HPP

/**
  * @class DyskryminatorSM
  * @author Wojciech Kogut
  *
  * Klasa służy do usuwania komentarzy z kodu asemblera, jest to podpieprzona i zuborzona wersja z projektu KWLiner.
  */

#include <stdexcept>
#include <QStringList>
#include <QPair>
#include <QVector>

class DyskryminatorSM
{
public:
    DyskryminatorSM();

    void ResetState();

    void PushChar(QChar sgn) throw(std::runtime_error);
    void PushEndl() throw(std::runtime_error);

    bool wasComment;
    QString outBuff;

private:
    QStringList oneLineComments{";", "//"}; // To jest kod podpieprzony z uniwersalnegodyskryminatora komentarzy, nie chcę mi się go
    QVector<QPair<QString, QString> > startEndComments{QPair<QString, QString>{"/*", "*/"}}; // optymalizować, niech zostanie tak.
    QVector<QPair<QString, QString> > stringInd{QPair<QString, QString>{"\"", "\\\""}};

    enum state
    {
        stateNormal,
        stateOneLineComment,
        statePermComment,
        stateString
    };

    state currentState;
    QString workBuff;
    QString awaitingDoubleEnd;
    QString awaitingStringException;
};

#endif
