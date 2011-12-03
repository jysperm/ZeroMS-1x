#ifndef INLINE_H
#define INLINE_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QString>

inline int QBtoint(QByteArray b)
{
    //从QByteArray向int转换
    QDataStream d(b);
    int i;
    d>>i;
    return i;
}

inline QByteArray inttoQB(int i)
{
    //从int向QByteArray转换
    QByteArray b;
    QDataStream d(b);
    d<<i;
    return b;
}

inline QString md5(QString s)
{
    //简写MD5操作
    return QString(QCryptographicHash::hash(s.toAscii(),QCryptographicHash::Md5).toHex());
}

#endif // INLINE_H
