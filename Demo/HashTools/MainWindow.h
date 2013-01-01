#ifndef ZEROMS_DEMO_HASHTOOLS_MAINWINDOW_H
#define ZEROMS_DEMO_HASHTOOLS_MAINWINDOW_H

#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

namespace ZeroMS {
namespace Demo {
namespace HashTools {

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
    void on_actionMD5_triggered();
    void on_actionSHA1_triggered();
    void on_actionSHA224_triggered();
    void on_actionSHA256_triggered();
    void on_actionSHA384_triggered();
    void on_actionSHA512_triggered();

private:
    Ui::MainWindow *ui;
};

}}}   //namespace ZeroMS::Demo::HashTools

#endif // ZEROMS_DEMO_HASHTOOLS_MAINWINDOW_H
