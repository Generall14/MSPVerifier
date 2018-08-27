#ifndef FILE_HPP
#define FILE_HPP

/**
  * @class File
  * @author Wojciech Kogut
  *
  * Klasa reprezentuje plik tekstowy zawierający kod asemblera MSP430 (*.s43). Ładuje dane ze wskazanego pliku oraz przetwrza je do postaci
  * ułatwiającej dalszą analizę. Posiada metodę dzielącą plik na sekcje opartą o dyrektywy rseg/aseg.
  */

#include "LineContainer.hpp"
#include <QString>

class SegmentList;

class File : public LineContainer
{
public:
    File(QString adress);

    void divide(SegmentList& slist);

private:
    QString _adress; /**<Adres pliku*/
    QString _name; /**<Nazwa pliku*/

    void loadFile();
    void skipWhiteSigns();
    void doPreprocessor();
    void removeComments();
    void expandDefines();
    void expandMacros();
    void expandRept();
    void removeUslessStuff();
    void checkIgnore();
};

#endif
