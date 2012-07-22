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
    void destroyLink();
    void reSetUI();
    void doLogin(bool isForce=false);
private slots:
    void on_doLogin_clicked();
    //事件过滤器，用于处理回车登录
    bool eventFilter(QObject *watched, QEvent *event);
    void timeout();
    void saveWidgetSize();
    void reSetWidgetSize();
private:
    Ui::LoginWidget *ui;
    QTcpSocket *conn;
    bool isCanenl;
    QAction *Act_isShowIp;
};

#endif // LOGINWIDGET_H
