#include "OClientCore.h"

OClientCore::OClientCore():isLoged(0),isShowIp(1),main(0),loginWidget(0),mainWidget(0)
{

}

OClientCore::~OClientCore()
{
    delete loginWidget;
    delete mainWidget;
    delete main;
}

void OClientCore::init()
{
    initLoginWidget();
}

void OClientCore::initLoginWidget()
{
    loginWidget=new LoginWidget;
    loginWidget->show();
}

void OClientCore::initMainWidget()
{
    QRect pos=loginWidget->geometry();
    QSize size=loginWidget->size();
    DELETE(loginWidget);
    mainWidget=new MainWidget;
    mainWidget->setGeometry(pos);
    mainWidget->resize(size);
    mainWidget->show();
}
