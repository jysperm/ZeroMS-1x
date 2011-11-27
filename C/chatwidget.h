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
private:
    Ui::ChatWidget *ui;
};

#endif // CHATWIDGET_H
