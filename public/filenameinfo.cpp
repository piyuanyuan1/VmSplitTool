#include "filenameinfo.h"

fileNameInfo::fileNameInfo(QStringList fileList,QObject *parent) : QObject(parent)
{
    fileNameList = fileList;
}

int fileNameInfo::getDuration(int index)
{
    QString fileName = fileNameList.at(index);
    int dPos = fileName.indexOf("D");
    int tPos = fileName.indexOf("T");
    QString durStr = fileName.mid(dPos+1,tPos-1-dPos);
    return durStr.toInt();
}
QStringList fileNameInfo::getTags(int index)
{
    QString fileName = fileNameList.at(index);
    QStringList tags;
    int begin = fileName.indexOf("T");
    int next = fileName.indexOf("#",begin+1);
    for(int i =0 ;i<fileName.count("#");i++)
    {
        tags.append(fileName.mid(begin+1,next-1-begin));
        begin = next;
        next = fileName.indexOf("#",begin+1);
        if(next<0)
            break;
    }
    return tags;
}
int fileNameInfo::getDuration(QString fileName)
{
    int dPos = fileName.indexOf("D");
    int tPos = fileName.indexOf("T");
    QString durStr = fileName.mid(dPos+1,tPos-1-dPos);
    return durStr.toInt();
}
QStringList fileNameInfo::getTags(QString fileName)
{
    QStringList tags;
    int begin = fileName.indexOf("T");
    int next = fileName.indexOf("#",begin+1);
    for(int i =0 ;i<fileName.count("#");i++)
    {
        tags.append(fileName.mid(begin+1,next-1-begin));
        begin = next;
        next = fileName.indexOf("#",begin+1);
        if(next<0)
            break;
    }
    return tags;
}

int fileNameInfo::getDurationByTag(QString tag)
{
    int duration = 0;
    for(int i =0 ;i<this->fileNameList.count();i++)
    {
        if(getTags(i).contains(tag))
            duration+=getDuration(i);
    }
    return duration;
}
