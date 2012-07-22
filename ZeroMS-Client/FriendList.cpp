#include "FriendList.h"
#include <QBoxLayout>

FriendList::FriendList(QWidget *parent):QScrollArea(parent)
{

}

void FriendList::init(QMap<QString,FriendListItem*> *map)
{
    this->map=map;

    layoutBox=new QBoxLayout(QBoxLayout::TopToBottom);

    layoutBox->setMargin(0);
    layoutBox->setContentsMargins(0,0,0,0);

    layoutBox->addItem(new QSpacerItem(20,40,QSizePolicy::Minimum,QSizePolicy::Expanding));

    QWidget *main=new QWidget;
    main->setLayout(layoutBox);

    main->setContentsMargins(0,0,0,0);

    this->setWidget(main);
}

FriendList::~FriendList()
{
    delete this->widget()->layout();
    delete this->widget();
}

void FriendList::addItem(OUserlistItem user)
{
    map->insert(user.uname,new FriendListItem(user));
    layoutBox->insertWidget(layoutBox->count()-1,map->value(user.uname));
}

bool FriendList::removeItem(QString uname)
{
    layoutBox->removeWidget(map->value(uname));
    delete map->value(uname);
    return map->remove(uname);
}

void FriendList::clear(bool isOnlineOnly)
{
    QMapIterator<QString,FriendListItem*> i(*map);
    while(i.hasNext())
    {
        i.next();
        if(i.value()->status()!=OFFLINE || !isOnlineOnly)
        {
            widget()->layout()->removeWidget(i.value());
            delete i.value();
            map->remove(i.key());
        }
    }
}
