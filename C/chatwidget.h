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
    explicit ChatWidget(QWidget *parent = 0);
    ~ChatWidget();
private:
    Ui::ChatWidget *ui;
};

#endif // CHATWIDGET_H
