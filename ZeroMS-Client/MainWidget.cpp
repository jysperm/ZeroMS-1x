#include "MainWidget.h"
#include "ui_MainWidget.h"

MainWidget::MainWidget():ui(new Ui::MainWidget)
{
    ui->setupUi(this);

}

MainWidget::~MainWidget()
{
    delete ui;
}
