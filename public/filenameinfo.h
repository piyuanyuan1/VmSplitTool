#ifndef FILENAMEINFO_H
#define FILENAMEINFO_H

#include <QObject>
#include <QString>
#include <QStringList>

class fileNameInfo : public QObject
{
    Q_OBJECT
public:
    explicit fileNameInfo(QStringList fileList,QObject *parent = nullptr);
    static int getDuration(QString fileName);
    static QStringList getTags(QString fileName);

    int getDuration(int index);
    QStringList getTags(int index);
    int getDurationByTag(QString tag);
signals:
private:
    QStringList fileNameList;


};

#endif // FILENAMEINFO_H
