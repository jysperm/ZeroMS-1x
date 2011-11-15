#include "servercore.h"

extern QTextStream cout;

ServerCore::ServerCore()
{
    //构造函数负责初始化，而监听由run()函数进行
    log(tr("0-ms start"));
    setMaxPendingConnections(SocketList_MAX);
}

void ServerCore::run()
{
    //开始监听
    log(tr("listening,port ")+QString::number(ServerCore_CLIENT_PORT));
    listen(QHostAddress::Any,ServerCore_CLIENT_PORT);
    connect(this,SIGNAL(newConnection()),this,SLOT(newConn()));
}

void ServerCore::stop()
{
    //终止监听并释放所有连接，请谨慎使用，这将导致所有用户掉线
    log(tr("0-ms stop"));
    close();
    QList<QString> keys=sl.keys();
    for(int i=0;i<keys.size();i++)
    {
        sl.remove(keys[i]);
    }
}

//--------下面私有函数
void ServerCore::log(QString msg)
{
    //负责打印日志，目前是输出到标准输出
    //建立这个函数的目的是为了方便今后修改输出的位置
    cout<<msg<<endl;
}

int ServerCore::QBtoint(QByteArray b)
{
    //从QByteArray向int转换
    QDataStream d(b);
    int i;
    d>>i;
    return i;
}

QByteArray ServerCore::inttoQB(int i)
{
    //从int向QByteArray转换
    QByteArray b;
    QDataStream d(b);
    d<<i;
    return b;
}

QString ServerCore::md5(QString s)
{
    //只是为了简写MD5操作
    return QString(QCryptographicHash::hash(s.toAscii(),QCryptographicHash::Md5).toHex());
}

void ServerCore::checkMsg(QString uname)
{
    //分发消息，该函数将根据消息的类型分发给具体的处理函数
    QByteArray *data=sl[uname]->data;
    //取前32位(协议版本)，注意只是复制，并没有从data中删除
    unsigned int ver=QBtoint(data->mid(0,4));
    unsigned int len=QBtoint(data->mid(4,4));
    if(!checkVer(ver))
    {
        throwError(uname);
        return;
    }
    //这是因为包头中的长度字段并不包含头部本身的长度
    sl[uname]->waitForLen=P1_HEAD_LEN+len;
    if(data->size() >= sl[uname]->waitForLen)
    {
        //如果已经接收到了全部数据，进行分发命令
        unsigned int type=QBtoint(data->mid(8,4));
        unsigned int time=QBtoint(data->mid(12,4));
        unsigned int len=QBtoint(data->mid(4,4));
        QByteArray *msgData=new QByteArray(data->mid(P1_HEAD_LEN,len));
        sl[uname]->lasttime=QDateTime::currentDateTime().toTime_t();
        switch(type)
        {
            case M1_Login:
                msgLogin(uname,msgData,time);
                break;
            case M1_AskTime:
                msgAckTime(uname,msgData,time);
                break;
	    case M1_AskUList:
		msgAskUList(uname,msgData,time);
		break;
            default:
                throwError(uname);
                return;
        }
        delete msgData;
        data->remove(0,P1_HEAD_LEN+len);
    }
}

void ServerCore::throwError(QString uname)
{
    //向客户端抛出异样，并关闭连接
    //一般是无法解析客户端发来的消息时，使用此函数
    log(tr("%1 abort，cant understand").arg(uname));
    QByteArray data;
    QByteArray msgData;
    //请注意，只能用QDataStream处理数字类型，如果处理字符串的话，会被转换成QT专有的格式，而非UTF-8
    QDataStream DSdata(&data,QIODevice::ReadWrite);
    msgData.append(ERROR_STR);
    unsigned int time=QDateTime::currentDateTime().toTime_t();
    DSdata<<P_USE<<msgData.size()<<M1_Error<<time;
    data.append(msgData);
    QTcpSocket *conn=sl[uname]->conn;
    conn->write(data);
    sl[uname]->isClose=1;
}

void ServerCore::closeClient(QString uname)
{
    //关闭到客户端的连接
    QTcpSocket *conn=sl[uname]->conn;
    conn->abort();
    if(sl[uname]->isLoged)
	msgChangeUList();
    sl.remove(uname);
}


//--------下面消息处理函数
void ServerCore::msgLogin(QString uname,QByteArray *data,unsigned int)
{

    QString msg=*data;
    //下面几句比较乱，总之是为了从数据部分(msg)里拆分出各个字段
    QString msguname=msg.left(msg.indexOf(" "));
    msg.remove(0,msguname.length()+1);
    QString msgpwd=msg.left(msg.indexOf(" "));
    msg.remove(0,msgpwd.length()+1);
    QString msgclientver=msg.left(msg.indexOf(" "));
    msg.remove(0,msgclientver.length()+1);
    QString msgclientname=msg.left(msg.indexOf(" "));
    log(tr("%1 logining").arg(uname));
    unsigned int stime=QDateTime::currentDateTime().toTime_t();
    QString dpwd=md5(API_KEY+msgpwd);
    QByteArray content;
    content.append(QString("do=login&uname=")+msguname+
                       QString("&listname=")+uname+
                       QString("&pwd=")+dpwd+
                       QString("&time=")+QString::number(stime)+
                       QString("&clientver=")+msgclientver+
                       QString("&clientname=")+msgclientname);
    manager= new QNetworkAccessManager(this);
    QNetworkRequest request ;
    request.setUrl(QUrl(LOGIN_APIURL));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,content.length());
    reply=manager->post(request,content);
    connect(reply, SIGNAL(finished()), this, SLOT(LoginResult()));
}

void ServerCore::msgAckTime(QString uname,QByteArray*,unsigned int)
{
    sl[uname]->lasttime=QDateTime::currentDateTime().toTime_t();
    msgTime(uname);
}

void ServerCore::msgTime(QString uname)
{
    QByteArray data;
    QByteArray msgData;
    QDataStream DSdata(&data,QIODevice::ReadWrite);
    QDataStream DSmsgData(&msgData,QIODevice::ReadWrite);

    unsigned int time=QDateTime::currentDateTime().toTime_t();
    msgData.append(QString::number(time));

    DSdata<<P_USE<<msgData.size()<<M1_Time<<time;
    data.append(msgData);

    QTcpSocket *conn=sl[uname]->conn;
    conn->write(data);
}

void ServerCore::msgChangeUList()
{
    log(tr("user list change"));
    QList<QString> keys=sl.keys();
    for(int i=0;i<keys.size();i++)
    {
	if(sl[keys[i]]->isLoged)
	{
	    qDebug()<<keys[i];
	    QByteArray data;
	    QByteArray msgData;
	    QDataStream DSdata(&data,QIODevice::ReadWrite);

	    unsigned int time=QDateTime::currentDateTime().toTime_t();

	    DSdata<<P_USE<<msgData.size()<<M1_ChangeUList<<time;
	    data.append(msgData);

	    QTcpSocket *conn=sl[keys[i]]->conn;
	    conn->write(data);
	}
    }
}

void ServerCore::msgUList(QString uname)
{
    QByteArray data;
    QByteArray msgData;
    QDataStream DSdata(&data,QIODevice::ReadWrite);

    unsigned int time=QDateTime::currentDateTime().toTime_t();

    QStringList users,tusers=sl.keys();
    QString i;
    foreach(i,tusers)
    {
	qDebug()<<(uname+i+QString::number(sl[i]->isLoged));
	if((sl[i]->isLoged))
	    users.append(i);
    }

    msgData.append(users.join(","));

    DSdata<<P_USE<<msgData.size()<<M1_UList<<time;
    data.append(msgData);

    QTcpSocket *conn=sl[uname]->conn;
    conn->write(data);
}

void ServerCore::msgAskUList(QString uname,QByteArray*,unsigned int)
{
    log(tr("%1 ask user list").arg(uname));
    if(sl[uname]->isLoged)
	msgUList(uname);
}

void ServerCore::msgLoginOk(QString uname)
{
    log(tr("%1 login ok").arg(uname));

    QByteArray data;
    QByteArray msgData;
    QDataStream DSdata(&data,QIODevice::ReadWrite);

    unsigned int time=QDateTime::currentDateTime().toTime_t();

    DSdata<<P_USE<<msgData.size()<<M1_LoginOk<<time;
    data.append(msgData);

    QTcpSocket *conn=sl[uname]->conn;
    conn->write(data);

}

void ServerCore::msgLoginError(QString uname)
{
    log(tr("%1 login error").arg(uname));
    QByteArray data;
    QByteArray msgData;
    QDataStream DSdata(&data,QIODevice::ReadWrite);

    unsigned int time=QDateTime::currentDateTime().toTime_t();

    DSdata<<P_USE<<msgData.size()<<M1_LoginError<<time;
    data.append(msgData);

    QTcpSocket *conn=sl[uname]->conn;
    conn->write(data);
}

//--------下面是槽函数
void ServerCore::LoginResult()
{
    QList<QString> result=QString::fromUtf8(reply->readAll()).split("\n");
    if(result[0].toInt())
    {
	if(sl.contains(result[UAPI_UNAME]))
	    closeClient(result[UAPI_UNAME]);
        sl.rename(result[UAPI_LISTNAME],result[UAPI_UNAME]);
        sl[result[UAPI_UNAME]]->isLoged=1;
	msgChangeUList();
        sl[result[UAPI_UNAME]]->lasttime=QDateTime::currentDateTime().toTime_t();
        sl[result[UAPI_UNAME]]->clientversion=result[UAPI_CLIENTVER].toInt();
        sl[result[UAPI_UNAME]]->clientname=result[UAPI_CLIENTNAME];
        msgLoginOk(result[UAPI_UNAME]);
    }
    else
    {
        msgLoginError(result[UAPI_LISTNAME]);
        sl[result[UAPI_LISTNAME]]->isClose=1;
    }
}

void ServerCore::newConn()
{
    //收到新连接
    while(hasPendingConnections())
    {
        QTcpSocket *pQTS=nextPendingConnection();
        //连接对象的命名格式请参考socketlist.h
        QString key="#"+pQTS->peerAddress().toString()+":"+QString::number(pQTS->peerPort());
        ClientConn *pCC=sl.add(key,pQTS);
        pCC->isLoged=0;
        //绑定信号槽
        connect(pQTS,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
        connect(pQTS,SIGNAL(readyRead()),this,SLOT(onData()));
	log(tr("%1 connected").arg(pCC->uname));
    }
}

void ServerCore::onData()
{
    //收到数据，是sl中各连接对象发出的
    //该函数把收到的数据存入ClientConn的缓存区(data)，
    //然后判断缓存区里是否是一个完整的消息，如果是就调用分发消息的checkMsg()函数
    //TODO,其实这里有些多此一举，因为TCP可以保证报文完整送达，
    //但是考虑可能有的消息可能比较大，需要多次发送，还是采用了这样的设计
    QList<QString> keys=sl.keys();
    for(int i=0;i<keys.size();i++)
    {
        if(sl[keys[i]]->isClose)
        {
            //如果已经被标记了已关闭(isClose),就删除这个连接
            closeClient(keys[i]);
            continue;
        }
        QTcpSocket *conn=sl[keys[i]]->conn;
        if(!conn->atEnd())
        {
            QByteArray *data=sl[keys[i]]->data;
            data->append(conn->readAll());
            if(data->size() >= sl[keys[i]]->waitForLen)
            {
                checkMsg(keys[i]);
            }
        }
    }
}

void ServerCore::onError(QAbstractSocket::SocketError s)
{
    //错误信息，是sl中各连接对象发出的
    QList<QString> keys=sl.keys();
    for(int i=0;i<keys.size();i++)
    {
        QTcpSocket *conn=sl[keys[i]]->conn;
        if(conn->error()==s)
        {
            //如果这个连接对象的上一个错误和接收到的错误号一样，
            //说明是它出错，输出错误信息并断开连接
	    log(tr("%1 connect error：%2").arg(keys[i],conn->errorString()));
            conn->abort();
            sl.remove(keys[i]);
	    msgChangeUList();
        }
    }
}
