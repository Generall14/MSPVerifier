#ifndef MSPFILE_HPP
#define MSPFILE_HPP

#include <QString>

//<TODO>
class MSPFile
{
public:
    MSPFile(QString adr);

    QString ToString() const;

private:
    QString _adr;
};

#endif
