#include "File.hpp"
#include <QTextStream>
#include <QFile>
#include <stdexcept>
#include "Logger.hpp"
#include <QFileInfo>
#include <QString>

File::File(QString adress):
    _adress(adress)
{
    loadFile();
    doPreprocessor();

    Logger::WriteFile("files/"+_name+".txt", this->toSString());
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
    {
        _lines.append(Line(_adress, l, cnt++));
    }

    file.close();
}

void File::skipWhiteSigns()
{
    for(int l=_lines.size()-1;l>=0;l--)
    {
        _lines[l].currentText=_lines[l].currentText.replace("\t", " ");
        while(_lines[l].currentText.indexOf("  ")>=0)
            _lines[l].currentText=_lines[l].currentText.replace("  ", " ");
        if(_lines[l].currentText.remove(" ").isEmpty())
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
//    for(int l=_lines.size()-1;l>=0;l--)
//    {
//        if(_lines.at(l).currentText.startsWith("#include \""))
//    }
}
