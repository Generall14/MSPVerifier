#ifndef FILE_HPP
#define FILE_HPP

#include "LineContainer.hpp"
#include <QString>

class File : public LineContainer
{
public:
    File(QString adress);

private:
    QString _adress;
    QString _name;
};

#endif
