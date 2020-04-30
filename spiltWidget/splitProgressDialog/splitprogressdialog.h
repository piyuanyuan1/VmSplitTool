#ifndef SPLITPROGRESSDIALOG_H
#define SPLITPROGRESSDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class splitProgressDialog;
}

class splitProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit splitProgressDialog(QWidget *parent = nullptr);
    ~splitProgressDialog();
    void dialogInit(int maxNum);
public slots:
    void setProgressValue(int value);
signals:
    void splitCanceled(bool isCanseled);
private:
    Ui::splitProgressDialog *ui;
    void closeEvent(QCloseEvent *e);
};

#endif // SPLITPROGRESSDIALOG_H
