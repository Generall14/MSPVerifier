#ifndef CONV_HPP
#define CONV_HPP

#include <QMap>

class Fun;
class Reg;

class Conv
{
    friend class Convs;
public:
    bool checkFun(const Fun& fun) const;
    QString name() const;
    QString toString() const;
    QMap<QString, Reg> prepareReturnedRegs() const;

private:
    void set(QString reg, QString type, QString size);
    Conv(QString name);

    enum type
    {
        dontTouch,
        touchAndRestore,
        touch
    };

    struct typeS
    {
        type _type=dontTouch;
        QString _size="w";
    };
    Reg prepare(const typeS& typ, QString reg) const;

    QString _name = "?????";
    QMap<QString, typeS> _types;
};

#endif
