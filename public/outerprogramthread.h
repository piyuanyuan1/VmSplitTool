#ifndef OUTERPROGRAMTHREAD_H
#define OUTERPROGRAMTHREAD_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include <QStringList>
#include <QFile>
#include <QCoreApplication>
#include <QString>
#include <QDebug>

class outerProgramThread : public QThread
{
    Q_OBJECT
public:
    explicit outerProgramThread(QObject *parent = nullptr);
    void threadInit(QString program,QStringList args,int timeout = -1);//初始化并启动线程，prongram为绝对路径
private:
    QString program;
    QStringList arguments;
    QProcess *process;
    int timeout;
    void run();
signals:
    void programFinished();
    void threadFinished();
    void programStarted();
    void standOutput(QString output);

};

#endif // OUTERPROGRAMTHREAD_H
