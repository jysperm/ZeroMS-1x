#include "Base/Auth/Hash.h"
#include "MainWindow.h"
#include "Base/Auth/RSA.h"

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

void MainWindow::on_actionMakeKeyPair_triggered()
{
    QPair<RSAPrivateKey,RSAPublicKey> pair=RSA::makeKeyPair();
    pair.first.print(stdout);
    pair.second.print(stdout);
}

}}}   // namespace ZeroMS::Demo::RSATools


