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

    inline void addItem(OUserlistItem user);
    inline bool removeItem(QString uname);
    inline FriendListItem *item(QString uname);
    void clear(bool isOnlineOnly=false);
private:
    QMap<QString,FriendListItem*> *map;
};

inline void FriendList::addItem(OUserlistItem user)
{
    map->insert(user.uname,new FriendListItem(user));
    widget()->layout()->addWidget(map->value(user.uname));
}

inline bool FriendList::removeItem(QString uname)
{
    widget()->layout()->removeWidget(map->value(uname));
    delete map->value(uname);
    return map->remove(uname);
}

inline FriendListItem *FriendList::item(QString uname)
{
    return map->value(uname);
}

#endif // FRIENDLIST_H
