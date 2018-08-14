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
    expandDefines();
    expandMacros();
    expandRept();
    removeUslessStuff();

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

void File::expandRept()
{
    skipWhiteSigns();
    bool found = false;

    do
    {
        found = false;

        int fi = -1;
        int si = -1;
        for(int i=0;i<_lines.size();i++)
        {
            if(fi<0)
            {
                if(_lines.at(i).currentText.indexOf(" rept ", Qt::CaseInsensitive)>=0) // <TODO> nieladne, zamienic na std::find
                {
                    fi = i;
                    continue;
                }
            }
            else
            {
                if(_lines.at(i).currentText.indexOf(" endr ", Qt::CaseInsensitive)>=0)
                {
                    si = i;
                    break;
                }
            }
        }

        if((fi>=0)&&(si<0))
            throw std::runtime_error("File::expandRept: nie zakonczone rept");

        if(fi>=0)
        {
            QString cnts = _lines.at(fi).currentText.split(" rept", QString::SkipEmptyParts, Qt::CaseInsensitive).at(0);
            bool ok;
            int cnt = cnts.toInt(&ok);
            if((!ok)||(cnt<=0))
                throw std::runtime_error("File::expandRept: bledna liczba powtorzen: \""+_lines.at(fi).currentText.toStdString()+"\"");

            QList<Line> tlines;
            for(int k=fi+1;k<si;k++)
                tlines.append(_lines.at(k));

            for(;si>=fi;si--)
                _lines.removeAt(si);

            while(cnt--)
            {
                for(int k=tlines.size()-1;k>=0;k--)
                    _lines.insert(fi, tlines.at(k));
            }

            found = true;
        }

    }while(found);
}

void File::expandDefines()
{
    skipWhiteSigns();
    bool found = false;

    do
    {
        found = false;

        QString idef="", rdef="";
        int idefi=0;

        for(int i=0;i<_lines.size();i++)
        {
            if(_lines.at(i).currentText.startsWith(" #define", Qt::CaseInsensitive))
            {
                QString temp = _lines.at(i).currentText;
                QStringList li = temp.remove(" #define ", Qt::CaseInsensitive).split(" ", QString::SkipEmptyParts);

                if(li.size()==0)
                    throw std::runtime_error("File::expandDefines: wtf? "+_lines.at(i).currentText.toStdString());

                idef = li.at(0);
                for(int k=1;k<li.size();k++)
                    rdef.append(li.at(k)+" ");

                found = true;
                idefi = i;
                break;
            }
        }

        if(!idef.isEmpty())
        {
            _lines.removeAt(idefi);
            if(!rdef.isEmpty())
            {
                for(auto it = _lines.begin();it!=_lines.end();it++)
                    it->currentText.replace(idef, rdef);
            }
        }

    }while(found);
}

void File::removeUslessStuff()
{
    skipWhiteSigns();
    for(int i=_lines.size()-1;i>=0;i--)
    {
        if((_lines.at(i).currentText.indexOf(" public ", Qt::CaseInsensitive)>=0)
                ||(_lines.at(i).currentText.indexOf(" extern ", Qt::CaseInsensitive)>=0)
                ||(_lines.at(i).currentText.indexOf(" end ", Qt::CaseInsensitive)>=0))
            _lines.removeAt(i);
    }
}

void File::divide(SegmentList& slist)
{
    int fi = 0, li  = 0;
    QString type = "unknown", current = "";
    for(int i=0;i<_lines.size();i++)
    {
        if((_lines.at(i).currentText.startsWith(" rseg ", Qt::CaseInsensitive)))
        {
            current = _lines.at(i).currentText;
            current.remove(" rseg ", Qt::CaseInsensitive);
            current = current.split("(", QString::SkipEmptyParts).first().split(" ", QString::SkipEmptyParts).first().remove(" ");
            current = current.toLower();
        }

        if(!current.isEmpty())
        {
            li = i;
            if(li==fi)
            {
                type = current;
                current = "";
                continue;
            }
            if(_lines.at(fi).currentText.indexOf(" aseg ", Qt::CaseInsensitive)<0)
                slist.append(Segment(_lines.mid(fi, li-fi), type));
            type = current;
            current = "";
            fi = li;
        }
    }
    if(fi<_lines.size()-1)
    {
        if(_lines.at(fi).currentText.indexOf(" aseg ", Qt::CaseInsensitive)<0)
            slist.append(Segment(_lines.mid(fi), type));
    }
}
