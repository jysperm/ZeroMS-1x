#ifndef PUBLIC_OGLOBAL_H
#define PUBLIC_OGLOBAL_H

#include <QCryptographicHash>
#include <QString>
#include "OGlobal/protocol2.0.h"
using namespace Protocol2;

//注意，如果要操作的指针是只读的，不要使用这个宏，直接用delete关键字就行
#define DELETE(p) {delete (p);(p)=0;}

//全局默认配置文件路径
const QString PUBLIC_CONFIG_FILE = QObject::trUtf8("./config.ini");

inline QString OSha1(QString s)
{
    return QString(QCryptographicHash::hash(s.toAscii(),QCryptographicHash::Sha1).toHex());
}

inline bool OIsGroup(QString uname)
{
    //检查一个用户名是否是小组名
    return uname.left(1)=="*";
}

inline QString OGroupName(QString uname)
{
    //去除小组名前面的星号
    return uname.right(uname.length());
}

inline QString OToGroup(QString group)
{
    //在小组名字前面加上星号
    return group.append("*");
}

#endif // PUBLIC_OGLOBAL_H
