#ifndef SPLITTHREAD_H
#define SPLITTHREAD_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QProcess>
#include <QList>
#include <QDate>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>

class splitThread : public QThread
{
    Q_OBJECT
public:
    explicit splitThread(QObject *parent = nullptr);

public slots:
    void threadInit(QList<double> startNodeList,QList<double> endNodeList,\
                    QList<QString> tagNodeList,\
                    QString saveDir,QString orginalFileName);
    void setTerminationFlag(bool flag);
private:
    void run();
    QList<double> startNodeList;
    QList<double> endNodeList;
    QList<QString> tagNodeList;
    QString saveDir;
    QString fileName;
    int finishedCount;
    QProcess *splitProcess;
    inline void splitTask();
    bool terminationFlag;
    QString convertTimeFormat(double time);//time单位为毫秒

private slots:

signals:
    void progress(int currentCount);
};

#endif // SPLITTHREAD_H
