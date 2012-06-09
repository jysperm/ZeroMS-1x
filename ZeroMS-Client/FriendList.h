#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QScrollArea>
#include "FriendListItem.h"

class FriendList : public QScrollArea
{
    Q_OBJECT
public:
    explicit FriendList(QWidget *parent = 0);
    ~FriendList();
    void init();

    inline void addItem(QString avatar,QString uname);
    inline bool removeItem(QString uname);
    inline FriendListItem *item(QString uname);
    void clear(bool onlineOnly=false);
private:
    QMap<QString,FriendListItem*> map;
};

inline void FriendList::addItem(QString avatar,QString uname)
{
    FriendListItem *item=new FriendListItem(avatar,uname);
    widget()->layout()->addWidget(item);
    map.insert(uname,item);
}

inline bool FriendList::removeItem(QString uname)
{
    widget()->layout()->removeWidget(map[uname]);
    delete map[uname];
    return map.remove(uname);
}

inline FriendListItem *FriendList::item(QString uname)
{
    return map[uname];
}

#endif // FRIENDLIST_H
