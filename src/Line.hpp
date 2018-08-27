#ifndef LINE_HPP
#define LINE_HPP

/**
  * @class Line
  * @author Wojciech Kogut
  *
  * Klasa reprezentuje linię tekstu z pliku wejściowego. Zawiera w sobię informacje o nazwie pliku, numerze lini w pliku, tekst pierwotny,
  * tekst przetworzony oraz wskaźnik na obiekt klasy Core reprezentującu stan rdzenia po wyknaniu instrukcji zawartej w dasnej linii. Klasa
  * moze być zawarta w obiektach pliku (File), w obiektach segmentów (Segment), w obiektach kodu (Code). Jedyną zawartą w klasie logiką jest
  * rozdzielanie kodu na etykiety, instrukcje i ragumenty. Pozostałe czynności wykonywane są przez inne klasy, obiekt Line jest traktowany
  * zwykła struktura na dane. Klasa nie posiada konstruktora łądującego dane z pliku, tym muszą się zająć inne obiekty wykorzystujące Line.
  */

#include <QString>
#include <QStringList>

class Core;

class Line
{
public:
    Line(QString file, QString text, int line);
    Line(const Line& old, QString newLine);
    Line(const Line& other);
    Line& operator=(const Line& other);
    ~Line();

    QString File() const;
    QString OryginalText() const;
    int LineNumber() const;

    QString toString() const;
    QString toSString() const;

    QString getInstruction() const;
    QString getInstructionSize() const;
    QString getLabel() const;
    QStringList getArguments() const;

    QString currentText; /**<Tekst po przetworzeniu*/
    Core* core = nullptr; /**<Wskaźnik na obiekt rdzenia przedstawiający stan po wykonaniu instrukcji, umieszczany tutaj w trakcie symulacji*/

private:
    int _line; /**<Numer lini w pierwotnym pliku*/
    QString _oText; /**<Tekst w pierwotnym pliku*/
    QString _file; /**<Nazwa pliku z którego pochodzi tekst*/
};

#endif
