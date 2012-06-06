#include "MainWidget.h"
#include "ui_MainWidget.h"
#include <QPushButton>
#include <QGroupBox>
#include <QMovie>
#include "FriendListItem.h"

MainWidget::MainWidget():ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    QSize size(ui->myAvatar->width(),ui->myAvatar->height());
    QMovie *avatar = new QMovie(":/images/0ms2logo.png");
    avatar->setScaledSize(size);
    ui->myAvatar->setMovie(avatar);
    avatar->start();

    ui->myName->setText("MyName");

    QBoxLayout *vlayout=new QBoxLayout(QBoxLayout::TopToBottom);

    vlayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    vlayout->setMargin(0);
    vlayout->setContentsMargins(0,0,0,0);

    for(int i=0;i<30;i++)
        vlayout->addWidget(new FriendListItem);

    QWidget *main=new QWidget;
    main->setLayout(vlayout);

    ui->friendList->setWidget(main);

}

MainWidget::~MainWidget()
{
    delete ui;
}
