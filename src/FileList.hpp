#ifndef FILELIST_HPP
#define FILELIST_HPP

#include <QString>
#include <QStringList>
#include "src/osrc/pugixml.hpp"

/**
 * Tworzy listę plików na podstawie podanego pliku *.ewp / *.ewt.
 */
class FileList
{
public:
    static QStringList GetFileList(QString inputfile);

private:
    FileList(){}

    static void iterateInNodes(pugi::xml_node& node, QStringList& list);
};

#endif
