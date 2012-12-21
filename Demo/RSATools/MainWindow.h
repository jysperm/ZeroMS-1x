#ifndef ZEROMS_DEMO_RSATOOLS_MAINWINDOW_H
#define ZEROMS_DEMO_RSATOOLS_MAINWINDOW_H

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
    
private slots:
    void on_actionMakeKeyPair_triggered();

private:
    Ui::MainWindow *ui;
};

}}}   //namespace ZeroMS::Demo::RSATools

#endif // ZEROMS_DEMO_RSATOOLS_MAINWINDOW_H
