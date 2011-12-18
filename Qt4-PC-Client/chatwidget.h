#ifndef CHATWIDGET_H
#define CHATWIDGET_H

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
    void reSetUi();
    void onMsg(QString msg);
    QString peerName;
signals:
    //详见oclientcoreex.h中的removeFromWidgets()函数
    void onDelete(QString uname);
private:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);//事件过滤器，用于处理回车发送
    Ui::ChatWidget *ui;
private slots:
    void on_DoSend_clicked();
    void on_ActPeerInfo_triggered();
};

#endif // CHATWIDGET_H
