#include "File.hpp"
#include <QTextStream>
#include <QFile>
#include <stdexcept>
#include "Logger.hpp"
#include <QFileInfo>

File::File(QString adress):
    _adress(adress)
{
    QFile file(adress);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("File::File: nie mozna otworzyc pliku \""+adress.toStdString()+"\"");

    _name = QFileInfo(file).baseName();

    QTextStream ts(&file);

    int cnt = 1;
    QString l;
    while(ts.readLineInto(&l))
    {
        _lines.append(Line(adress, l, cnt++));
    }

    file.close();

    Logger::WriteFile("files/"+_name+".txt", this->toSString());
}
