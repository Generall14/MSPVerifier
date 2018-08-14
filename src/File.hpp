#ifndef FILE_HPP
#define FILE_HPP

#include "LineContainer.hpp"
#include <QString>
#include "segmentList.hpp"

class File : public LineContainer
{
public:
    File(QString adress);

    void divide(SegmentList& slist);

private:
    QString _adress;
    QString _name;

    void loadFile();
    void skipWhiteSigns();
    void doPreprocessor();
    void removeComments();
    void expandDefines();
    void expandMacros();
    void expandRept();
    void removeUslessStuff();
};

#endif
