#ifndef SPLITWIDGET_H
#define SPLITWIDGET_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QString>
#include <QProcess>
#include <QDebug>
#include <QList>
#include <QListWidgetItem>
#include <QAction>
#include <QAbstractItemView>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QDate>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QDir>

#include "splitthread.h"
#include "splitProgressDialog/splitprogressdialog.h"
#include "../tagEditWidget/tageditwidget.h"
#include "../public/outerprogramthread.h"
#include "../tagChooceWidget/tagchoocewidget.h"
#include "../generateWidget/generatewidget.h"
#include "../formatCovertWidget/formatcovertwidget.h"

namespace Ui {
class splitWidget;
}
enum currentState {spliting,detectting,idle};
enum detectMode{correct,generate};

class splitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit splitWidget(QWidget *parent = nullptr);
    ~splitWidget();
private slots:
    void openFileSlot();//打开文件，并初始化软件
    void playBtnSlot();
    void posChangeSlot(qint64 newPos);
    void sliderMovedSlot(int newValue);//进度条移动槽函数
    void editSliderMovedSlot(int newValue);//编辑进度条移动槽函数
    void durChangedSlot(qint64 newDuration);//当前视频长度更新
    void markBtnSLot();//添加分割节点
    void splitBtnSlot();
    void splitFinished();//视频分割结束
    void splitProgress(int progress);//更新分割任务进度
    void actSlot();//所有QAction的槽函数
    void listSelChangedSlot();
    void openToolWidget();
    void listDoubleClickedSlot(QListWidgetItem *item);
    void tagChooceDoneSot(QString tag,QPoint pos);
    void scrollCangedSlot(int value);
    void fragmentDetect();//检测视频片段，初始化参数并启动线程
    void detectFinishedSlot(QString output);//片段检测(python脚本)结束

private:
    Ui::splitWidget *ui;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    qint64 VideoDuration;//单位为毫秒
    qint64 videoPos;//单位毫秒
    int sliderValue;
    QString fileName;
    QList<double> startNodeList;
    QList<double> endNodeList;
    QList<QString> tagNodeList;
    QString saveDir;
    QString totalCountStr;
    tagChooceWidget *myTagDialog;
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void debugeOut(QString debugInfo);//输出提示信息
    void listItemFlagSetup();//设置QListWidget中Item的Flags
    bool eventFilter(QObject *obj, QEvent *event);
    inline void videoPlay(double pos);//指定位置播放视频，单位为毫秒
    inline void connectSetup();//设置信号槽连接
    class splitThread *mySplitThread;
    class outerProgramThread *myDetectThread;
    splitProgressDialog *splitProgressDialog;//用于显示分割任务进度
    QList<QAction*> LWActList;
    QListWidget *onContextWidget;
    inline double numberMatch(double number,QList<double> data);//在给定data中匹配与number差值最小的，并返回
    void loadSaveFile();//加载存档文件
    void autoSave();//存档
    detectMode detectMode;
    currentState currentState;
};

#endif // SPLITWIDGET_H
