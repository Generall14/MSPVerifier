#ifndef FILELIST_HPP
#define FILELIST_HPP

/**
  * @class FileList
  * @author Wojciech Kogut
  *
  * Klasa odczytuje listę plików *.s43 z pliku konfiguracyjnego projektu IAR (.ewp/.ewt), ładuje je wszystkie do obiektów File. Posiada
  * metodę wyciągającą z odczytanych plików obiekt SegmentList zawierający zbiór segmentów.
  */

#include <QString>
#include <QStringList>
#include "src/osrc/pugixml.hpp"
#include "File.hpp"
#include <QList>

class SegmentList;

class FileList
{
public:
    FileList(QString inputfile);

    SegmentList getSegments();

private:
    void iterateInNodes(pugi::xml_node& node, QStringList& list);
    void SearchFiles(QString inputfile);
    void LoadFiles();

    QList<File> _list; /**<Lista odczytanych plików.*/
    QStringList _files; /**<Lista adresów plików.*/
};

#endif
