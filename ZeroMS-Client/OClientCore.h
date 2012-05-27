#ifndef OCLIENTCORE_H
#define OCLIENTCORE_H

#include <QObject>
#include "LoginWidget.h"

class OClientCore:public QObject
{
    Q_OBJECT
public:
    explicit OClientCore();
    void init();
    void initLoginWidget();

    LoginWidget *loginWidget;
};

#endif // OCLIENTCORE_H
