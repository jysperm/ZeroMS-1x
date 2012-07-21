#include <QMovie>
#include "FriendListItem.h"
#include "ui_FriendListItem.h"

FriendListItem::FriendListItem(OUserlistItem info):ui(new Ui::FriendListItem),info(info)
{
    ui->setupUi(this);

    if(info.avatar.isEmpty())
        info.avatar=":/images/0ms2logo.png";

    QSize size(ui->avatar->width(),ui->avatar->height());
    QMovie *movie = new QMovie(info.avatar);
    movie->setScaledSize(size);
    ui->avatar->setMovie(movie);
    movie->start();

    setContentsMargins(0,0,0,0);

    setUName(info.uname);
}

FriendListItem::~FriendListItem()
{
    delete ui->avatar->movie();
    delete ui;
}
