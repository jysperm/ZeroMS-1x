#include "OClientCore.h"

OClientCore::OClientCore():isLoged(0),isShowIp(1),main(0),loginWidgeta(0)
{

}

void OClientCore::init()
{
    initLoginWidget();
}

void OClientCore::initLoginWidget()
{
    loginWidgeta=new LoginWidget;
    loginWidgeta->show();
}
