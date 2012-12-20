#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

namespace ZeroMS {
namespace Demo {
namespace RSATools {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
};

}}}   //namespace ZeroMS::Demo::RSATools

#endif // MAINWINDOW_H
