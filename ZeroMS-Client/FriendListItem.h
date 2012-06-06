#ifndef FRIENDLISTITEM_H
#define FRIENDLISTITEM_H

#include <QWidget>
#include <QMovie>
#include <QFile>
#include "ui_FriendListItem.h"

namespace Ui
{
class FriendListItem;
}

class FriendListItem : public QWidget
{
    Q_OBJECT
public:
    explicit FriendListItem(QString avatar=QString(":/images/0ms2logo.png"),QString uname=QString());
    ~FriendListItem();

    inline QString avatar();
    inline QString uname();
    inline void setAvatar(QString avatar);
    inline void setUName(QString uname);
private:
    Ui::FriendListItem *ui;
    QString avatarFile;
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
    ui->avatar->movie()->setFileName(avatar);
}

inline void FriendListItem::setUName(QString uname)
{
    ui->uname->setText(uname);
}

#endif // FRIENDLISTITEM_H
