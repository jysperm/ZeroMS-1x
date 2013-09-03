#ifndef LOGIN_H
#define LOGIN_H

#include "const.h"
#include "OClientCoreEx.h"
class QString;
class OClientCoreEx;

namespace Ui
{
    class Login;
}

class Login:public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    void reSetUi();
    //登陆成功的回调函数(由OClientCoreEx调用)，用于处理记住密码功能
    void onLoginOK();
public slots:
    //取消登陆
    void cancel();
private:
    Ui::Login *ui;
    //是否是“记住密码”模式
    int isRemembered;
private slots:
    void QLable_linkActivated(const QString &link);
    void LoginError();
    void on_DoLogin_clicked();
    void on_Options_clicked();
    void on_UserInput_textEdited(const QString &text);
};

#endif // LOGIN_H
