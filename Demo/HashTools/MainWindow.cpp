#include "MainWindow.h"

namespace ZeroMS {
namespace Demo {
namespace HashTools {

#include "ui_MainWindow.h"

}}}   // namespace ZeroMS::Demo::HashTools

#include "Base/Auth/Hash.h"

using namespace ZeroMS::Base::Auth;
using namespace ZeroMS::Demo::HashTools;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new ::Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionMD5_triggered()
{
    ui->hashResult->append(MD5::md5(ui->source->toPlainText()));
}

void MainWindow::on_actionSHA1_triggered()
{
    ui->hashResult->append(SHA::sha(ui->source->toPlainText(),SHA::SHA1));
}

void MainWindow::on_actionSHA224_triggered()
{
    ui->hashResult->append(SHA::sha(ui->source->toPlainText(),SHA::SHA224));
}

void MainWindow::on_actionSHA256_triggered()
{
    ui->hashResult->append(SHA::sha(ui->source->toPlainText(),SHA::SHA256));
}

void MainWindow::on_actionSHA384_triggered()
{
    ui->hashResult->append(SHA::sha(ui->source->toPlainText(),SHA::SHA384));
}

void MainWindow::on_actionSHA512_triggered()
{
    ui->hashResult->append(SHA::sha(ui->source->toPlainText(),SHA::SHA512));
}
