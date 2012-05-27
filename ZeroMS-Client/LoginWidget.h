#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "global.h"
#include "OClientCore.h"

namespace Ui
{
class LoginWidget;
}

class LoginWidget:public QWidget
{
    Q_OBJECT
    
public:
    explicit LoginWidget();
    ~LoginWidget();
    void cancel();
    
private slots:
    void on_DoLogin_clicked();

private:
    Ui::LoginWidget *ui;
    QTcpSocket *conn;
};

#endif // LOGINWIDGET_H
