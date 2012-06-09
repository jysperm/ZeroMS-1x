#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>

namespace Ui
{
class MainWidget;
}

class MainWidget : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWidget();
    ~MainWidget();
    Ui::MainWidget *ui;
private slots:
    void on_actRefreshFriendList_triggered();

private:

};

#endif // MAINWIDGET_H
