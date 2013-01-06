#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

namespace ZeroMS {
namespace Demo {
namespace BSONTools {

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

}}}   //namespace ZeroMS::Demo::BSONTools

#endif // MAINWINDOW_H
