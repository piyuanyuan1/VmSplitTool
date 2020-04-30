#include "splitprogressdialog.h"
#include "ui_splitprogressdialog.h"

splitProgressDialog::splitProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::splitProgressDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
}

splitProgressDialog::~splitProgressDialog()
{
    delete ui;
}
void splitProgressDialog::setProgressValue(int value)
{
    ui->progressBar->setValue(value);
}
void splitProgressDialog::closeEvent(QCloseEvent *e)
{
    emit splitCanceled(true);
    this->done(1);
}
void splitProgressDialog::dialogInit(int maxNum)
{
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(maxNum);
    this->exec();
}
