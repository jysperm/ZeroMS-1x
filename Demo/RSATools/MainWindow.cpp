#include "Base/Auth/Hash.h"
#include "Base/Auth/RSA.h"
#include "MainWindow.h"

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
    QString filename=QFileDialog::getSaveFileName(this,tr("保存私钥"));

    if(!filename.isEmpty())
    {
        QFile f(filename);
        f.open(QFile::WriteOnly);
        f.write(this->priKey.toPEM());
        f.close();
    }
}

void MainWindow::on_actionSavePubKeyToFile_triggered()
{
    QString filename=QFileDialog::getSaveFileName(this,tr("保存公钥"));

    if(!filename.isEmpty())
    {
        QFile f(filename);
        f.open(QFile::WriteOnly);
        f.write(this->pubKey.toPEM());
        f.close();
    }
}

void MainWindow::on_actionReadPriKeyFromFile_triggered()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("读取私钥"));

    if(!filename.isEmpty())
    {
        QFile f(filename);
        f.open(QFile::ReadOnly);
        this->priKey=RSAPrivateKey::fromPEM(f.readAll());
        f.close();
    }
}

void MainWindow::on_actionReadPubKeyFromFile_triggered()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("读取公钥"));

    if(!filename.isEmpty())
    {
        QFile f(filename);
        f.open(QFile::ReadOnly);
        this->pubKey=RSAPublicKey::fromPEM(f.readAll());
        f.close();
    }
}

void MainWindow::on_actionValiy_triggered()
{
    //TODO：当私钥无效时(即刚打开程序时), 会直接崩溃.
    QMessageBox::information(this,tr("验证私钥"),(this->priKey.isValid()?tr("私钥有效"):tr("私钥无效")));
}

void MainWindow::on_actionPriKeyInfo_triggered()
{
    this->ui->textOut->append(this->priKey.print());
}

void MainWindow::on_actionPubKeyInfo_triggered()
{
    this->ui->textOut->append(this->pubKey.print());
}

}}}   // namespace ZeroMS::Demo::RSATools