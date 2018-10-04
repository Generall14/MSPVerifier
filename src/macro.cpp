#include "macro.hpp"
#include <QString>
#include <stdexcept>
#include "File.hpp"
#include "Line.hpp"
#include <qDebug>
#include <iostream>

volatile int Macro::localCounter=0;

/**
 * Metoda odczytuje definicje makr ze wskazanego pliku File i zwraca ich listę. Usuwa ze wskazanego pliku tekst definicji makra.
 */
QList<Macro> Macro::loadMacros(File& file)
{
    QList<Macro> temp;
    bool found = false;

    do
    {
        found = false;

        int fi = -1;
        int si = -1;
        for(int i=0;i<file.get().size();i++)
        {
            if(fi<0)
            {
                if(file.get().at(i).currentText.indexOf(" macro ", Qt::CaseInsensitive)>=0) // <TODO> nieladne, zamienic na std::find
                {
                    fi = i;
                    continue;
                }
            }
            else
            {
                if(file.get().at(i).currentText.indexOf(" endm ", Qt::CaseInsensitive)>=0)
                {
                    si = i;
                    break;
                }
            }
        }

        if((fi>=0)&&(si<0))
            throw std::runtime_error("Macro::loadMacros: nie zakonczone makro");

        if(fi>=0)
        {
            Macro m;
            QStringList sp = file.get().at(fi).currentText.split(" macro ", QString::SkipEmptyParts, Qt::CaseInsensitive);
            m._name = sp.first().remove(" ");
            if(sp.size()>1)
                m._args = sp.at(1).split(",", QString::SkipEmptyParts, Qt::CaseInsensitive);
            for(auto it=m._args.begin();it!=m._args.end();it++)
                it->remove(" ");

            for(int i=fi+1;i<si;i++)
                m._lines.append(file.get().at(i).currentText);

            for(;si>=fi;si--)
                file.get().removeAt(si);

            found = true;
            m.loadLocals();
            temp.append(m);
        }

    }while(found);

    return temp;
}

/**
 * Rozwija makro we wskazanym obiekcie File.
 */
bool Macro::applyMacro(File& file)
{
    bool found = false, retstate = false;

    do
    {
        if(found)
            retstate = true;
        found = false;
        Macro::localCounter++;
        for(int i=0;i<file.get().size();i++)
        {
            if(i>0)
            {
                if(file.get().at(i-1).currentText.contains(";##ignorenextline"))
                    continue;
            }
            if(file.get().at(i).currentText.indexOf(" "+_name+" ", Qt::CaseInsensitive)>=0)
            {
                QString temp = file.get().at(i).currentText;
                QStringList pars = temp.remove(_name).split(",", QString::SkipEmptyParts);
                for(int k=pars.size()-1;k>=0;k--)
                {
                    pars[k].remove(" ");
                    if(pars.at(k).isEmpty())
                        pars.removeAt(k);
                }
                if(pars.size()!=_args.size())
                    throw std::runtime_error("Macro::applyMacro: niezgodna lista parametrow w makrze "+_name.toStdString()+" w "+
                                             file.get().at(i).toString().toStdString());

                for(int k=_lines.size()-1;k>=0;k--)
                {
                    QString newlocal;
                    QString tline = _lines.at(k);
                    for(QString loc: _locals)
                    {
                        newlocal = loc+"_"+QString::number(Macro::localCounter);
                        tline.replace(loc, newlocal, Qt::CaseInsensitive);
                    }
                    for(int x=0;x<pars.size();x++)
                        tline.replace(_args.at(x), pars.at(x), Qt::CaseSensitive);
                    file.get().insert(i+1, Line(file.get().at(i), tline));
                }

                file.get().removeAt(i);

                found = true;
                break;
            }
        }

    }while(found);
    return retstate;
}

void Macro::loadLocals()
{
    for(int i=_lines.size()-1;i>=0;i--)
    {
        if(_lines.at(i).contains(" local ", Qt::CaseInsensitive))
        {
            QString temp = _lines.at(i).split(" local ", QString::SkipEmptyParts).at(0);
            _locals.append(temp.remove(" "));
            _lines.removeAt(i);
        }
    }
}
