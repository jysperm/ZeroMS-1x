#include "const.h"
#include "oclientcoreex.h"
#include "login.h"

//public:
OClientCoreEx::OClientCoreEx():login(0)
{

}

OClientCoreEx::~OClientCoreEx()
{

}

void OClientCoreEx::init()
{
    login=new Login;
    login->show();
}
