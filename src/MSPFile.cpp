#include "MSPFile.hpp"

MSPFile::MSPFile(QString adr):
    _adr(adr)
{

}

QString MSPFile::ToString() const
{
    //<TODO>
    QString temp = "File at "+_adr+"\n";
    return temp;
}
