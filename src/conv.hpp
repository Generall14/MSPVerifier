#ifndef CONV_HPP
#define CONV_HPP

#include <QMap>

class Fun;

class Conv
{
    friend class Convs;
public:
    bool checkFun(const Fun& fun) const;
    QString name() const;
    QString toString() const;

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

    QString _name = "?????";
    QMap<QString, typeS> _types;
};

#endif
