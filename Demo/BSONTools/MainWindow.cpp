#include "MainWindow.h"

namespace ZeroMS {
namespace Demo {
namespace BSONTools {

#include "ui_MainWindow.h"

using namespace ::ZeroMS::Demo::BSONTools;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

}}}   // namespace ZeroMS::Demo::BSONTools
