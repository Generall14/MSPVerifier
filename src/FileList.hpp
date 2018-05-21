#ifndef FILELIST_HPP
#define FILELIST_HPP

#include <QString>
#include <QStringList>
#include <QDir>

/**
 * Klasa tworzy listę plików z katalogu root (rekurencyjnie po podkatalogach) z końcówkami podanymi w
 * suffixes. Pliki i katalogi zawierające dowolny z tekstów podanych w excludes zostaną zignorowane.
 * Porównywanie końcówek i wykluczeń odbywa się z ignorowaniem wielkości znaków.
 */
class FileList
{
public:
    FileList(QString root = "./", QStringList suffixes = QStringList(), QStringList excludes = QStringList());

    QStringList GetFileList();

private:
    QStringList list;
    bool searched = false;

    QString _root;
    QStringList _suff;
    QStringList _excl;

    void SearchFiles();
    void IterateInFiles(QString dir);
};

#endif
