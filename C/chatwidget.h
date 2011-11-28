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
    void closeEvent(QCloseEvent *event);
    Ui::ChatWidget *ui;
private slots:
    void on_DoSend_clicked();
};

#endif // CHATWIDGET_H
