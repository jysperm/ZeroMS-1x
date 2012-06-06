#include "FriendList.h"
#include <QBoxLayout>

FriendList::FriendList(QWidget *parent):QScrollArea(parent)
{

}

void FriendList::init()
{
    QBoxLayout *layoutBox=new QBoxLayout(QBoxLayout::TopToBottom);

    layoutBox->setMargin(0);

    for(int i=0;i<30;i++)
        layoutBox->addWidget(new FriendListItem);

    QWidget *main=new QWidget;
    main->setLayout(layoutBox);

    this->setWidget(main);
}

FriendList::~FriendList()
{
    delete this->widget()->layout();
    delete this->widget();
}
