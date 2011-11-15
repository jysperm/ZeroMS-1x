#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "const.h"
class QString;

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
public slots:
    //取消登陆
    void cancel();
    void LoginError();
private:
    Ui::Login *ui;
private slots:
    void QLable_linkActivated(const QString &link);
    void on_DoLogin_clicked();
};

#endif // LOGIN_H
