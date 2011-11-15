#include "socketlist.h"

SocketList::SocketList()
{

}

//已登录的连接对象的键名是用户名
//未登录的连接对象的键名是井号+IP+冒号+端口，如#192.168.1.101:65534
ClientConn *SocketList::add(QString keyname,QTcpSocket *conn)
{
    //添加连接对象，如果成功返回新添加对象的指针，如果已存在同名对象则返回该对象的指针
    //失败返回0(一般是超出最大数量)
    if(size()>=SocketList_MAX)
	return 0;
    remove(keyname);
    ClientConn *p=new ClientConn;
    p->uname=keyname;
    p->conn=conn;
    p->data=new QByteArray;
    insert(keyname,p);
    return p;
}

ClientConn *SocketList::rename(QString oldname,QString newname)
{
    //更改键名,用于从未登录状态到登陆状态时更改键名
    if(!contains(oldname))
        return 0;
    QTcpSocket *conn=value(oldname)->conn;
    remove(oldname);
    remove(newname);
    add(newname,conn);
    value(newname)->uname=newname;
    return value(newname);
}

int SocketList::remove(QString keyname)
{
    //删除连接对象，返回是否执行了删除(是否有这个对象)，该函数自动释放内存
    if(!contains(keyname))
	return 0;
    delete value(keyname);
    QMap::remove(keyname);
    return 1;
}
