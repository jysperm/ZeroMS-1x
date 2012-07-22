#ifndef FRIENDLISTITEM_H
#define FRIENDLISTITEM_H

#include <QWidget>
#include <QMovie>
#include <QFile>
#include "ui_FriendListItem.h"
#include "OServerPeer.h"

namespace Ui
{

class FriendListItem;

}

class FriendListItem : public QWidget
{
    Q_OBJECT
public:
    explicit FriendListItem(OUserlistItem info);
    ~FriendListItem();

    FriendListItem &operator =(OUserlistItem info);

    inline QString avatar();
    inline QString uname();
    inline void setAvatar(QString avatar);
    inline void setUName(QString uname);
    inline void setStatus(QString status);
    inline QString status();
    inline QString ip();
    inline void setIp(QString ip);
    inline QVector<int> p2pPorts();
    inline void setP2pPorts(QVector<int> ports);
private:
    Ui::FriendListItem *ui;
    OUserlistItem info;
};

inline QString FriendListItem::avatar()
{
    return ui->avatar->movie()->fileName();
}

inline QString FriendListItem::uname()
{
    return ui->uname->text();
}

inline void FriendListItem::setAvatar(QString avatar)
{
    info.avatar=avatar;
    ui->avatar->movie()->setFileName(avatar);
}

inline void FriendListItem::setUName(QString uname)
{
    info.uname=uname;
    ui->uname->setText(uname);
}

inline void FriendListItem::setStatus(QString status)
{
    info.status=status;
    ui->isOnline->setChecked(!(status==OFFLINE));
}

inline QString FriendListItem::status()
{
    return info.status;
}

inline QString FriendListItem::ip()
{
    return info.ip;
}

inline void FriendListItem::setIp(QString ip)
{
    info.ip=ip;
}

inline QVector<int> FriendListItem::p2pPorts()
{
    return info.p2pPorts;
}

inline void FriendListItem::setP2pPorts(QVector<int> ports)
{
    info.p2pPorts=ports;
}

#endif // FRIENDLISTITEM_H
