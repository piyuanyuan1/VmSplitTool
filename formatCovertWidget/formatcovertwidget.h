#ifndef FORMATCOVERTWIDGET_H
#define FORMATCOVERTWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QApplication>

#include "../public/outerprogramthread.h"

namespace Ui {
class formatCovertWidget;
}

class formatCovertWidget : public QWidget
{
    Q_OBJECT

public:
    explicit formatCovertWidget(QWidget *parent = nullptr);
    ~formatCovertWidget();
private slots:
    void openBtnSlot();
    void converBtnSlot();
    void converFinishedSlot();
private:
    Ui::formatCovertWidget *ui;
    QString fileName;
    QString getOffExtension(QString filename);
    outerProgramThread *ffmpegThread;
};

#endif // FORMATCOVERTWIDGET_H
