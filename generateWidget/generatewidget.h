#ifndef GENERATEWIDGET_H
#define GENERATEWIDGET_H

#include <QDialog>
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QTextBrowser>
#include <QCloseEvent>
#include <QAction>
#include <QTime>
#include <QWidget>

#include "../public/filenameinfo.h"

namespace Ui {
class generateWidget;
}

class generateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit generateWidget(QWidget *parent = nullptr);
    ~generateWidget();
private slots:
    void openDirSlot();
    void generateBtnSlot();
    void textChangedSlot(const QString newPath);
private:
    Ui::generateWidget *ui;
    void dirInfoDebug(QTextBrowser* borwser,QString dirPath);
    QAction *clearAct;
};

#endif // GENERATEWIDGET_H
