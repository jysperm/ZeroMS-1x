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

    inline QString avatar();
    inline QString uname();
    inline void setAvatar(QString avatar);
    inline void setUName(QString uname);
    inline void setStatus(QString status);
    inline QString status();

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

#endif // FRIENDLISTITEM_H
