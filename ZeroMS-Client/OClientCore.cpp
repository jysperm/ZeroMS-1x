#include "OClientCore.h"

OClientCore::OClientCore()
{

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
