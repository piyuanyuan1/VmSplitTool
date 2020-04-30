#include "renamethread.h"

renameThread::renameThread(QObject *parent) : QThread(parent)
{

}
void renameThread::threadInit(QStringList fileList, QStringList tagList, QString collectionStr, QString saveDir)
{
    this->fileNameList = fileList;
    this->tagList = tagList;
    this->collectionStr = collectionStr;
    this->saveDir = saveDir;
    this->start();
}
void renameThread::run()
{
    process = new QProcess();
    for(int i = 0;i<fileNameList.count();i++)
    {
        QString inputName = fileNameList.at(i);
        QString tag = tagList.at(i);
        QString ffprobe = QCoreApplication::applicationDirPath()+"/ffmpeg/bin/ffprobe.exe";
        QStringList args;
        args<<"-show_format"<<inputName;
        process->start(ffprobe,args);
        process->waitForStarted();
        process->waitForFinished(60000*30);
        QString videoInfo = QString::fromLocal8Bit(process->readAllStandardOutput());
        int startPos = videoInfo.indexOf("duration=")+9;
        int endPos = videoInfo.indexOf("\r\n",startPos);
        QString lenStr = videoInfo.mid(startPos,endPos-startPos);
        QString dateStr = QDate::currentDate().toString("yyyy.MM.dd").right(8);
        QString outputName = saveDir+"/"+dateStr+"D"+lenStr+"T"+tag+".mp4";
        int serialNumber = 1;
        QString numStr = "";
        while (QFile::exists(outputName))
        {
            serialNumber+=1;
            numStr = "."+QString::number(serialNumber);
            outputName = saveDir+"/"+dateStr+"D"+lenStr+"T"+tag+numStr+".mp4";
        }
        if(!outputName.contains("Collection#"))
            outputName = saveDir+"/"+dateStr+"D"+lenStr+"T"+tag+collectionStr+numStr+".mp4";
        QFile::rename(inputName,outputName);
        emit renameProgress(i+1);
    }
    emit renameFinished();
    delete process;
}
