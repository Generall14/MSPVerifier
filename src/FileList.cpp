#include "FileList.hpp"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "Logger.hpp"
#include "segmentList.hpp"
#include <iostream>

/**
 * Konstruktor przyjmujący adres pliku projektu IAR (.ewp/.ewt).
 * @param inputfile
 */
FileList::FileList(QString inputfile)
{
    SearchFiles(inputfile);
    LoadFiles();
}

/**
 * Wyciąga listę plików z pliku projektu zapisanego w formacie xml.
 */
void FileList::SearchFiles(QString inputfile)
{
    // Otwieranie pliku xml:
    pugi::xml_document file;
    pugi::xml_parse_result result =  file.load_file(inputfile.toStdString().c_str());

    if(!result)
        throw std::runtime_error("FileList::GetFileList: nie mozna otworzyc pliku XML \""+inputfile.toStdString()+
                                 "\": "+result.description()+", offset: "+
                                 QString::number(result.offset).toStdString()+" B");

    for(auto child: file.children())
        iterateInNodes(child, _files);

    QFileInfo conf(inputfile);
    for(QStringList::iterator it = _files.begin(); it!=_files.end(); it++)
    {
        it->replace("$PROJ_DIR$", conf.dir().path());
        it->replace("\\", "/");
    }

    QString total;
    for(QString s: _files)
        total += s + "\n";
    Logger::WriteFile("files/_listFiles.txt", total);

    Logger::Log("Znaleziono "+QString::number(_files.size())+" plików...");
}

void FileList::iterateInNodes(pugi::xml_node& node, QStringList& list)
{

    if(QString(node.name())=="file")
    {
        pugi::xml_node name = node.child("name");
        if(!name.empty())
        {
            if(QString(name.text().as_string()).endsWith(".s43"))
                list.append(name.text().as_string());
        }
    }

    for(auto child: node.children())
        iterateInNodes(child, list);
}

/**
 * Ładuje pliki zawarte w liście _files.
 */
void FileList::LoadFiles()
{
    for(auto file: _files)
        _list.append(File(file));
    Logger::Log("... i odczytano wszystkie");
}

/**
 * Dzieli listę plików na segmenty i zwraca obiekt SegmentList zawierający zbiór tych segmentów.
 */
SegmentList FileList::getSegments()
{
    Logger::Log("Dzielenie kodu na segmenty...");
    SegmentList temp;

    for(auto f: _list)
        f.divide(temp);

    return temp;
}
