#include "splitthread.h"

splitThread::splitThread(QObject *parent) : QThread(parent)
{

}

void splitThread::run()
{
    splitProcess = new QProcess();
    while (1)
    {
        if(startNodeList.count()<=0 || terminationFlag)
            break;
        splitTask();
        bool isFinished = splitProcess->waitForFinished(60000*30);
        if(isFinished)
        {
            finishedCount+=1;
            emit progress(finishedCount);
        }
    }
    delete splitProcess;
}
void splitThread::threadInit(QList<double> startNodeList, QList<double> endNodeList, \
                             QList<QString> tagNodeList, \
                             QString saveDir, QString orginalFileName)
{
    this->startNodeList = startNodeList;
    this->endNodeList = endNodeList;
    this->tagNodeList = tagNodeList;
    this->saveDir = saveDir;
    this->fileName = orginalFileName;
    finishedCount = 0;
    terminationFlag = false;
    this->start();
}
void splitThread::splitTask()
{
    QString ffmpeg = QCoreApplication::applicationDirPath()+"/ffmpeg/bin/ffmpeg.exe";
    if(startNodeList.isEmpty() || endNodeList.isEmpty() ||tagNodeList.isEmpty() || !QFile::exists(ffmpeg))
    {
        terminationFlag = true;
        return;
    }
    QString dateStr;
    double startNum = startNodeList.at(0);
    double lenNum = endNodeList.at(0)-startNodeList.at(0);
    dateStr = QDate::currentDate().toString("yyyy.MM.dd").right(8);
    QString outputName = saveDir+"/"+dateStr+"D"+QString::number(int(lenNum))+"T"+tagNodeList.at(0)+".mp4";
    int serialNumber = 1;
    while (QFile::exists(outputName))
    {
        serialNumber+=1;
        QString numStr = "."+QString::number(serialNumber);
        outputName = saveDir+"/"+dateStr+"D"+QString::number(int(lenNum))+"T"+tagNodeList.at(0)+numStr+".mp4";
    }
    QStringList arguments;
    arguments << "-i" << fileName<<"-ss"<<convertTimeFormat(startNum)<< "-t" << convertTimeFormat(lenNum)<< outputName<<"-y";
    startNodeList.removeAt(0);
    endNodeList.removeAt(0);
    tagNodeList.removeAt(0);
    if(lenNum > 0)
        splitProcess->start(ffmpeg,arguments);
}
QString splitThread::convertTimeFormat(double time)//time单位为毫秒
{
    int ms = int((time-int(time))*1000);
    int ss = int(time)%60;
    int mm = (int(time)/60)%60;
    int hh = (int(time)/60)/60;
    return QString::number(hh)+":"+QString::number(mm)+":"+QString::number(ss)+"."+QString::number(ms);
}
void splitThread::setTerminationFlag(bool flag)
{
    this->terminationFlag = flag;
}
