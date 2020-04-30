#include "generatewidget.h"
#include "ui_generatewidget.h"

generateWidget::generateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::generateWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Window);
    ui->textBrowser->setContextMenuPolicy(Qt::ActionsContextMenu);
    clearAct = new QAction("clear");
    ui->textBrowser->addAction(clearAct);
    connect(ui->sourceButton,SIGNAL(clicked()),this,SLOT(openDirSlot()));
    connect(ui->targetButton,SIGNAL(clicked()),this,SLOT(openDirSlot()));
    connect(ui->generateButton,SIGNAL(clicked()),this,SLOT(generateBtnSlot()));
    connect(ui->sourceEdit,SIGNAL(textChanged(const QString)),this,SLOT(textChangedSlot(const QString)));
    connect(clearAct,SIGNAL(triggered()),ui->textBrowser,SLOT(clear()));
}

generateWidget::~generateWidget()
{
    delete ui;
    delete clearAct;
}
void generateWidget::openDirSlot()
{
    QString path = "";
    path = QFileDialog::getExistingDirectory(this,"选择文件夹","F:/");
    if(path.isEmpty())
        return;
    if(this->sender()->objectName() == ui->sourceButton->objectName())
        ui->sourceEdit->setText(path);
    else if(this->sender()->objectName() == ui->targetButton->objectName())
        ui->targetEdit->setText(path);
}
void generateWidget::textChangedSlot(const QString newPath)
{
    ui->textBrowser->clear();
    dirInfoDebug(ui->textBrowser,newPath);
}
void generateWidget::generateBtnSlot()
{
    QDir souceDir(ui->sourceEdit->text());
    QDir targetDir(ui->tagEdit->text());
    if(!souceDir.exists() || ui->sourceEdit->text().isEmpty())
    {
        ui->textBrowser->append("<p style =\"color:gray;\">Source directory is invalid</p>");
        return;
    }
    if(ui->targetEdit->text().isEmpty())
    {
        ui->textBrowser->append("<p style =\"color:gray;\">Target diretory is invalid</p>");
        return;
    }
    if(!targetDir.exists())
    {
        bool isDone;
        isDone = targetDir.mkpath(ui->tagEdit->text());
        if(isDone == false)
        {
            ui->textBrowser->append("<p style =\"color:gray;\">Target diretory is invalid</p>");
            return;
        }
    }
    QStringList nameFilters;
    nameFilters << "*.mp4";
    QStringList files = souceDir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    if(files.count()<=0)
    {
        ui->textBrowser->append("<p style =\"color:gray;\">There is no file in Source directory</p>");
        return;
    }
    class fileNameInfo infoList(files);
    QStringList tagFilted;
    if(ui->tagEdit->text() == "")
        tagFilted = files;
    else
    {
        for(int i = 0; i<files.count(); i++)
        {
            if(infoList.getTags(i).contains(ui->tagEdit->text()))
                tagFilted.append(files.at(i));
        }
    }
    if(tagFilted.count()<=0)
    {
        ui->textBrowser->append("<p style =\"color:gray;\">No video file found with tag \""+ui->tagEdit->text()+"\"</p>");
        return;
    }
    class fileNameInfo lastInfoList(tagFilted);
    QStringList outputList;
    int currenrDur = 0;
    qsrand(QTime::currentTime().second());
    for(int index = 0;currenrDur/60.0 <= ui->durationEdit->text().toInt(); index++)
    {
        if(tagFilted.isEmpty())
            break;
        int randIndex = qrand()%tagFilted.count();
        currenrDur += fileNameInfo::getDuration(tagFilted.at(randIndex));
        outputList.append(tagFilted.takeAt(randIndex));
        qDebug()<<randIndex;
    }
    for(int i=0; i<outputList.count();i++)
        QFile::rename(ui->sourceEdit->text()+"/"+outputList.at(i),ui->targetEdit->text()+"/"+outputList.at(i));
    dirInfoDebug(ui->textBrowser,ui->targetEdit->text());
}
void generateWidget::dirInfoDebug(QTextBrowser* borwser,QString dirPath)
{

    QDir souceDir(dirPath);
    QStringList nameFilters;
    nameFilters << "*.mp4";
    QStringList files = souceDir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    class fileNameInfo infoList(files);
    int totalDuration = 0;
    for(int i = 0 ; i<files.count(); i++)
        totalDuration +=infoList.getDuration(i);
    QStringList tags;
    for(int i = 0; i < files.count();i++)
        tags.append(infoList.getTags(files.at(i)));
    QStringList allTag;
    for(int i = 0; i < tags.count();i++)
    {
        if(!allTag.contains(tags.at(i)))
            allTag.append(tags.at(i));
    }
    QString tempStr = "<span style = \"color:#fe970b;\">"+QString::number(totalDuration/60.0,'g',2)+"</span>";
    QString tempStr1 = "<span style = \"color:#fe970b;\">"+QString::number(files.count())+"</span> Files,  "+\
            tempStr+" min";
    borwser->append("<h3>"+dirPath+"("+tempStr1+")</h3>");
    for(int i = 0; i < allTag.count();i++)
    {
        QString teamp = "<span style = \"color:#fe970b;\">"+QString::number(infoList.getDurationByTag(allTag.at(i))/60.0,'g',2)+"</span>";
        QString countStr = "<span style = \"color:#fe970b;\">"+QString::number(tags.count(allTag.at(i)))+"</span>";
        borwser->append("<span style=\"color:#00af43;white-space:pre;\">    "+allTag.at(i)+"</span> : "+countStr+" files,  "+teamp+" min");
    }

}
