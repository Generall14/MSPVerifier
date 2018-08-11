#include "FileList.hpp"
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include "Logger.hpp"

QStringList FileList::GetFileList(QString inputfile)
{
    QStringList temp;

    // Otwieranie pliku xml:
    pugi::xml_document file;
    pugi::xml_parse_result result =  file.load_file(inputfile.toStdString().c_str());

    if(!result)
        throw std::runtime_error("FileList::GetFileList: nie mozna otworzyc pliku XML \""+inputfile.toStdString()+
                                 "\": "+result.description()+", offset: "+
                                 QString::number(result.offset).toStdString()+" B");

    for(auto child: file.children())
        iterateInNodes(child, temp);

    QFileInfo conf(inputfile);
    for(QStringList::iterator it = temp.begin(); it!=temp.end(); it++)
    {
        it->replace("$PROJ_DIR$", conf.dir().path());
        it->replace("\\", "/");
    }

    QString total;
    for(QString s: temp)
        total += s + "\n";
    Logger::WriteFile("_listFiles.txt", total);

    return temp;
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
