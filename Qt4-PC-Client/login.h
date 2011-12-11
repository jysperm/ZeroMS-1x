#ifndef LOGIN_H
#define LOGIN_H

#include <QAbstractSocket>
#include <QWidget>
#include "const.h"
#include "oclientcoreex.h"
class QString;
class OClientCoreEx;

namespace Ui
{
    class Login;
}

class Login:public QWidget
{
    Q_OBJECT
    friend class OClientCoreEx;
public:
    explicit Login(QWidget *parent = 0);
    ~Login();
public slots:
    //取消登陆
    void cancel();
private:
    Ui::Login *ui;
private slots:
    void QLable_linkActivated(const QString &link);
    void LoginError();
    void on_DoLogin_clicked();
    void on_Options_clicked();
};

#endif // LOGIN_H
