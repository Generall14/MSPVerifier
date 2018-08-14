#include "File.hpp"
#include <QTextStream>
#include <QFile>
#include <stdexcept>
#include "Logger.hpp"
#include <QFileInfo>
#include <QString>
#include <QDebug>
#include <QDir>
#include "dyskryminator.hpp"
#include "macro.hpp"

File::File(QString adress):
    _adress(adress)
{
    loadFile();
    Logger::WriteFile("files/"+_name+".txt", this->toSString());

    doPreprocessor();
    removeComments();
    expandMacros();

    skipWhiteSigns();
    Logger::WriteFile("parsedSFiles/"+_name+".txt", this->toSString());
    Logger::WriteFile("parsedFiles/"+_name+".txt", this->toString());
}

void File::loadFile()
{
    QFile file(_adress);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("File::File: nie mozna otworzyc pliku \""+_adress.toStdString()+"\"");

    _name = QFileInfo(file).baseName();

    QTextStream ts(&file);
    int cnt = 1;
    QString l;
    while(ts.readLineInto(&l))
        _lines.append(Line(_adress, l, cnt++));

    file.close();
}

void File::skipWhiteSigns()
{
    for(int l=_lines.size()-1;l>=0;l--)
    {
        _lines[l].currentText.replace("\t", " ");
        while(_lines[l].currentText.indexOf("  ")>=0)
            _lines[l].currentText.replace("  ", " ");
        if(_lines[l].currentText.isEmpty()||_lines[l].currentText==" ")
        {
            _lines.removeAt(l);
            continue;
        }
        if(!_lines[l].currentText.startsWith(" "))
            _lines[l].currentText.insert(0, " ");
        if(!_lines[l].currentText.endsWith(" "))
            _lines[l].currentText.append(" ");
    }
}

void File::doPreprocessor()
{
    skipWhiteSigns();
    for(int l=_lines.size()-1;l>=0;l--)
    {
        if(_lines.at(l).currentText.startsWith(" #include \"", Qt::CaseInsensitive))
        {
            int fi = _lines.at(l).currentText.indexOf("\"");
            int li = _lines.at(l).currentText.lastIndexOf("\"");
            QString name = _lines.at(l).currentText.mid(fi+1,li-fi-1);
            if(name!="msp430.h")
            {
                QString iadr = QFileInfo(_adress).dir().path()+"/"+name;

                QFile file(iadr);
                if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    throw std::runtime_error("File::doPreprocessor: nie mozna otworzyc pliku \""+iadr.toStdString()+"\"");

                QTextStream ts(&file);
                QString lt;
                int cnt = l+1;
                while(ts.readLineInto(&lt))
                    _lines.insert(cnt++, Line(_lines.at(l), lt));
                file.close();
            }
            _lines.removeAt(l);
        }
    }
}

void File::removeComments()
{
    skipWhiteSigns();
    Dyskryminator d;
    for(auto it=_lines.begin();it!=_lines.end();it++)
    {
        if(it->currentText.startsWith(" ;##"))
            continue;
        d.DyscriminateLine(it->currentText);
    }
}

void File::expandMacros()
{
    skipWhiteSigns();
    QList<Macro> macs = Macro::loadMacros(*this);
    for(auto mac: macs)
        mac.applyMacro(*this);
}
