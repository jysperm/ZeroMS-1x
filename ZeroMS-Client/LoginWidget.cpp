#include <QMovie>
#include <QDebug>
#include "LoginWidget.h"
#include "ui_LoginWidget.h"

LoginWidget::LoginWidget():ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    qDebug()<<QImageReader::supportedImageFormats();

    QSize size(ui->Avatar->width(),ui->Avatar->height());
    QMovie *avatar = new QMovie(":/images/0ms2logo.png");
    avatar->setScaledSize(size);
    ui->Avatar->setMovie(avatar);
    avatar->start();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}
