#include "FileList.hpp"
#include <QDir>
#include <QDebug>
#include "Logger.hpp"

/**
 * @param root - adres katalogu ze źródłami
 * @param suffixes - końcówki plików do odczytu
 * @param excludes - jeżeli w nazwie pliku lub katalogu znajdzie się tekst z tej listy - plik/katalog zostanie zignorowany
 */
FileList::FileList(QString root, QStringList suffixes, QStringList excludes):
    _root(root),
    _suff(suffixes),
    _excl(excludes)
{}

QStringList FileList::GetFileList()
{
    if(!searched)
        SearchFiles();
    return list;
}

void FileList::SearchFiles()
{
    searched = true;

    IterateInFiles(_root);

    QString txt;
    for(auto str: list)
        txt.append(str+"\n");
    Logger::WriteFile("Files.txt", txt);

    if(list.isEmpty())
        throw std::runtime_error( QString("Nie odnaleziono żadnych pasujących plików w katalogu "+_root).toStdString());
}

void FileList::IterateInFiles(QString dir)
{
    for(QString e: _excl)
    {
        if(dir.contains(e, Qt::CaseInsensitive))
            return;
    }

    QDir current(dir);
    if(!current.exists())
        throw std::runtime_error( QString("Katalog "+dir+" nie istnieje").toStdString() );

    for(QString f: current.entryList(QDir::Files))
    {
        bool ex = false;
        bool mat = false;
        if(_suff.isEmpty())
            mat = true;
        for(QString e: _excl)
        {
            if(f.contains(e, Qt::CaseInsensitive))
            {
                ex = true;
                continue;
            }
        }
        for(QString e: _suff)
        {
            if(f.endsWith(e, Qt::CaseInsensitive))
            {
                mat = true;
                continue;
            }
        }
        if((mat) && (!ex))
            list.append(dir+f);
    }

    for(QString str: current.entryList(QDir::Dirs|QDir::NoDotDot|QDir::NoDot))
    {

        IterateInFiles(dir+str+"/");
    }
}
