#ifndef TAGCHOOCEWIDGET_H
#define TAGCHOOCEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QString>
#include <QCursor>
#include <QShowEvent>
#include <QGraphicsDropShadowEffect>
#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QStringList>

namespace Ui {
class tagChooceWidget;
}

class tagChooceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tagChooceWidget(QWidget *parent = nullptr);
    ~tagChooceWidget();
    QString getCurrentTag();
private slots:
    void itemClickedSlot(QListWidgetItem *item);
    void timeoutSlot();
    void loadListFile();
private:
    Ui::tagChooceWidget *ui;
    QString currentTag;
    QGraphicsDropShadowEffect *shadow_effect;
    void showEvent(QShowEvent *event);
    QTimer timer;
signals:
    void selDone(QString tag,QPoint pos);
};

#endif // TAGCHOOCEWIDGET_H
