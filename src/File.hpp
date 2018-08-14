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

    void loadFile();
    void skipWhiteSigns();
    void doPreprocessor();
    void removeComments();
    void expandMacros();
};

#endif
