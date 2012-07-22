#include <QMovie>
#include "FriendListItem.h"
#include "ui_FriendListItem.h"

FriendListItem::FriendListItem(OUserlistItem info):ui(new Ui::FriendListItem)
{
    ui->setupUi(this);


    QMovie *movie = new QMovie;
    QSize size(ui->avatar->width(),ui->avatar->height());
    movie->setScaledSize(size);
    ui->avatar->setMovie(movie);

    operator =(info);

    setContentsMargins(0,0,0,0);
}

FriendListItem::~FriendListItem()
{
    delete ui->avatar->movie();
    delete ui;
}

FriendListItem &FriendListItem::operator =(OUserlistItem info)
{
    this->info=info;

    if(info.avatar.isEmpty())
        info.avatar=":/images/0ms2logo.png";

    ui->avatar->movie()->setFileName(info.avatar);
    ui->avatar->movie()->start();

    setUName(info.uname);
    setStatus(info.status);

    return *this;
}
