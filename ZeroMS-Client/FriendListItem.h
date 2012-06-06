#ifndef FRIENDLISTITEM_H
#define FRIENDLISTITEM_H

#include <QWidget>

namespace Ui {
class FriendListItem;
}

class FriendListItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit FriendListItem(QWidget *parent = 0);
    ~FriendListItem();
    
private:
    Ui::FriendListItem *ui;
};

#endif // FRIENDLISTITEM_H
