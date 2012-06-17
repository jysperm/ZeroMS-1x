#include "FriendList.h"
#include <QBoxLayout>

FriendList::FriendList(QWidget *parent):QScrollArea(parent)
{

}

void FriendList::init()
{
    QBoxLayout *layoutBox=new QBoxLayout(QBoxLayout::TopToBottom);

    layoutBox->setMargin(0);
    layoutBox->setContentsMargins(0,0,0,0);

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

void FriendList::clear(bool onlineOnly)
{
    QMapIterator<QString,FriendListItem*> i(map);
    while(i.hasNext())
    {
        i.next();
        if(i.value()->isOnline() || !onlineOnly)
        {
            widget()->layout()->removeWidget(i.value());
            delete i.value();
            map.remove(i.key());
        }
    }
}
