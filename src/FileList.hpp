#ifndef FILELIST_HPP
#define FILELIST_HPP

#include <QString>
#include <QStringList>
#include "src/osrc/pugixml.hpp"
#include "File.hpp"
#include <QList>

class SegmentList;

/**
 * Tworzy listę plików na podstawie podanego pliku *.ewp / *.ewt.
 */
class FileList
{
public:
    FileList(QString inputfile);

    SegmentList getSegments();

private:
    void iterateInNodes(pugi::xml_node& node, QStringList& list);
    void SearchFiles(QString inputfile);
    void LoadFiles();

    QList<File> _list;
    QStringList _files;
};

#endif
