#ifndef PUBLIC_OGLOBAL_H
#define PUBLIC_OGLOBAL_H

#include <QCryptographicHash>
#include <QString>
#include <QStringList>
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
    if(OIsGroup(uname))
        return uname.right(uname.length()-1);
    return uname;
}

inline QString OToGroup(QString group)
{
    //在小组名字前面加上星号
    if(!OIsGroup(group))
        return group.append("*");
    return group;
}

inline QString OSBEscSMap(QString str)
{
    //按照SB协议的键值对应表，转义分号
    return str.replace(";","\\;");
}

inline QString OGet(const QStringList &list,int index)
{
    //获得QStringList中指定索引的字符串，如果不存在这个索引返回空字符串
    if(list.count()>index)
        return list[index];
    else
        return QString();
}

#endif // PUBLIC_OGLOBAL_H
