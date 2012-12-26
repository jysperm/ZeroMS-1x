#include "Base/Auth/Hash.h"
#include "MainWindow.h"
#include "Base/Auth/RSA.h"

namespace ZeroMS {
namespace Demo {
namespace RSATools {

#include "ui_MainWindow.h"

using namespace ::ZeroMS::Base::Auth;
using namespace ::ZeroMS::Demo::RSATools;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow),isHasKeyPair(false)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionMakeKeyPair_triggered()
{
    QPair<RSAPrivateKey,RSAPublicKey> pair=RSAKeyMaker::makeKeyPair();

    this->isHasKeyPair=true;

    this->priKey=pair.first;
    this->pubKey=pair.second;
}

void MainWindow::on_actionSavePriKeyToFile_triggered()
{
    if(!this->isHasKeyPair)
    {
        QMessageBox::critical(this,tr("错误"),tr("还没载入密钥对"));
        return;
    }
    QString filename=QFileDialog::getSaveFileName(this,tr("保存私钥"));

    QFile f(filename);
    f.open(QFile::WriteOnly);
    f.write(this->priKey.toPEM());
    f.close();
}

void MainWindow::on_actionSavePubKeyToFile_triggered()
{
    if(!this->isHasKeyPair)
    {
        QMessageBox::critical(this,tr("错误"),tr("还没载入密钥对"));
        return;
    }
    QString filename=QFileDialog::getSaveFileName(this,tr("保存公钥"));

    QFile f(filename);
    f.open(QFile::WriteOnly);
    f.write(this->pubKey.toPEM());
    f.close();
}

}}}   // namespace ZeroMS::Demo::RSATools
