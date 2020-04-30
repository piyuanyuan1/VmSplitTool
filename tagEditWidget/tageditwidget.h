#ifndef TAGEDITWIDGET_H
#define TAGEDITWIDGET_H

#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QScrollBar>
#include <QApplication>
#include <QDesktopWidget>

#include "../public/filenameinfo.h"
#include "../tagChooceWidget/tagchoocewidget.h"
#include "renamethread.h"

namespace Ui {
class tagEditWidget;
}

class tagEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tagEditWidget(QWidget *parent = nullptr);
    ~tagEditWidget();
private slots:
    void confirmBtnSlot();
    void sourceChangedSlot(QString newPath);
    void openDirSlot();
    void tagChooceDoneSot(QString tag,QPoint pos);
    void listDoubleClickedSlot(QListWidgetItem *item);
    void scrollCangedSlot(int value);
    void listSelChangedSlot();
    void renameFinishedSlot();
private:
    Ui::tagEditWidget *ui;
    tagChooceWidget *myTagDialog;
    bool eventFilter(QObject *obj, QEvent *event);
    QString collectionStr;
    renameThread *renameThread;
    QStringList fileNameList;
    QStringList tagList;
    int currentCount;
};

#endif // TAGEDITWIDGET_H
