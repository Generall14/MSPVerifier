#ifndef REG_HPP
#define REG_HPP

#include <QString>
#include <QStringList>

class Reg
{
public:
    Reg(QString val, QString size="w");

    QString toString() const;
    bool merge(const Reg& other);

    static const QStringList regs;

private:
    QString _a, _w, _b;
};

#endif
