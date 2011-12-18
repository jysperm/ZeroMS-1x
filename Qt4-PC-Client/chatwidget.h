#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QString>
#include <QMainWindow>

namespace Ui
{
    class ChatWidget;
}

class ChatWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChatWidget(QString uname,QWidget *parent = 0);
    ~ChatWidget();
    bool eventFilter(QObject *watched, QEvent *event);//事件过滤器，用于处理回车发送
    void closeEvent(QCloseEvent *event);
    Ui::ChatWidget *ui;
    QString peerName;
    void onMsg(QString msg);
    void reSetUi();
signals:
    //详见oclientcoreex.h中的removeFromWidgets()函数
    void onDelete(QString uname);
private slots:
    void on_DoSend_clicked();
};

#endif // CHATWIDGET_H
