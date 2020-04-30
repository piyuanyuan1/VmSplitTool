#ifndef RENAMETHREAD_H
#define RENAMETHREAD_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDate>
#include <QProcess>
#include <QString>
#include <QCoreApplication>

class renameThread : public QThread
{
    Q_OBJECT
public:
    explicit renameThread(QObject *parent = nullptr);
    void threadInit(QStringList fileList,QStringList tagList,QString collectionStr,QString saveDir);
private:
    void run();
    QProcess *process;
    QStringList fileNameList;
    QStringList tagList;
    QString collectionStr;
    QString saveDir;
signals:
    void renameFinished();
    void renameProgress(int progress);
};

#endif // RENAMETHREAD_H
