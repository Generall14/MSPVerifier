#include "segmentList.hpp"
#include "Logger.hpp"
#include "fun.hpp"
#include "funContainer.hpp"

/**
 * Dodaje segment do bazy danych.
 */
void SegmentList::append(Segment seg)
{
    _seg[seg.type()].append(seg);

    if(seg.type()=="unknown")
        Logger::LogWarning("Nie rozpoznano sekcji w pliku \""+seg.get().at(0).File()+"\". Patrz sections/unknown.txt.");
}

/**
 * Zwraca segmenty poszczeregowae według typu.
 */
QMap<QString, QList<Segment> > &SegmentList::get()
{
    return _seg;
}

/**
 * Zapisuje bazę danych do plików w folderze outputs/sections*.
 */
void SegmentList::store()
{
    for(QString key: _seg.keys())
    {
        QString temp;
        for(auto seg: _seg[key])
        {
            temp.append(seg.toString());
            temp.append("\n\n\n\n\n\n");
        }

        Logger::WriteFile("sections/"+key+".csv", temp);
    }
}

/**
 * Odczytuje z segmentów code funkcje (według znacznika ;##fun) i zwraca ich listę jako FunContainer.
 */
FunContainer SegmentList::digForFunctions(bool quiet) const
{
    FunContainer fc;

    if(!_seg.contains("code"))
        throw std::runtime_error("SegmentList::digForFunctions: brak sekcji code");

    QList<Segment> codes = _seg["code"];

    for(Segment code: codes)
    {
        int fi = -1;
        for(int i=0;i<code.get().size();i++)
        {
            if((code.get().at(i).currentText.startsWith(" ;##fun", Qt::CaseInsensitive)))
            {
                if((fi==-1)&&(i>0))
                {
                    if(!quiet)
                        Logger::LogWarning("Nieoznaczony kod funkcji w pliku \""+code.get().at(0).File()+
                                       "\". Patrz code/unknown.txt.");
                    QString ss;
                    for(int k=0;k<i;k++)
                        ss.append(code.get().at(k).toString()+"\n");
                    if(!quiet)
                        Logger::AppendFile("code/unknown.txt", ss);
                }
                else if(fi>=0)
                    fc.append(Fun(code.get().mid(fi, i-fi)));
                fi = i;
            }
        }

        if(fi==-1)
        {
            if(!quiet)
                Logger::LogWarning("Nieoznaczony kod funkcji w pliku \""+code.get().at(0).File()+
                               "\". Patrz code/unknown.txt.");
            QString ss;
            for(int k=0;k<code.get().size();k++)
                ss.append(code.get().at(k).toString()+"\n");
            if(!quiet)
                Logger::AppendFile("code/unknown.txt", ss);
        }
        else if((fi+1)!=code.get().size())
            fc.append(Fun(code.get().mid(fi)));
    }

    return fc;
}
