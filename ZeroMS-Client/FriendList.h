#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QScrollArea>
#include "FriendListItem.h"
#include "OServerPeer.h"

class FriendList : public QScrollArea
{
    Q_OBJECT
public:
    explicit FriendList(QWidget *parent);
    ~FriendList();
    void init(QMap<QString,FriendListItem*> *map);

    void addItem(OUserlistItem user);
    bool removeItem(QString uname);
    inline FriendListItem *item(QString uname);
    void clear(bool isOnlineOnly=false);
private:
    QMap<QString,FriendListItem*> *map;
    QBoxLayout *layoutBox;
};

inline FriendListItem *FriendList::item(QString uname)
{
    return map->value(uname);
}

#endif // FRIENDLIST_H
