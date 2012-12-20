#include "Base/Auth/Hash.h"
#include "MainWindow.h"

namespace ZeroMS {
namespace Demo {
namespace RSATools {

#include "ui_MainWindow.h"

using namespace ::ZeroMS::Base::Auth;
using namespace ::ZeroMS::Demo::RSATools;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
}}}   // namespace ZeroMS::Demo::RSATools
