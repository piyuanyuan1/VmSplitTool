#include "splitwidget.h"
#include "ui_splitwidget.h"

splitWidget::splitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::splitWidget)
{
    ui->setupUi(this);
    this->layout()->setContentsMargins(10,0,10,10);
    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(ui->playWidget);
    myTagDialog = new class tagChooceWidget();
    mySplitThread = new class splitThread();
    myDetectThread = new class outerProgramThread();
    splitProgressDialog = new class splitProgressDialog();

    videoWidget->resize(ui->playWidget->size());
    videoWidget->setCursor(QCursor(Qt::PointingHandCursor));
    ui->startListWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->endListWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    LWActList<<ui->actionMerge<<ui->actionDivide<<ui->actionDelete<<ui->actionEdit\
             <<ui->actionClear<<ui->actionDetect<<ui->actionLoadSave<<ui->actionDetectFill;
    ui->startListWidget->addActions(LWActList);
    ui->endListWidget->addActions(LWActList);

    onContextWidget = nullptr;
    videoPos = 0;
    currentState = currentState::idle;
    ui->startListWidget->installEventFilter(this);
    ui->tagListWidget->installEventFilter(this);
    ui->endListWidget->installEventFilter(this);
    connectSetup();
}

splitWidget::~splitWidget()
{
    delete ui;
    delete player;
    delete videoWidget;
    delete myTagDialog;
    delete mySplitThread;
    delete splitProgressDialog;
    delete myDetectThread;
}
void splitWidget::connectSetup()
{
    connect(splitProgressDialog,SIGNAL(splitCanceled(bool)),mySplitThread,SLOT(setTerminationFlag(bool)));
    connect(myTagDialog,SIGNAL(selDone(QString,QPoint)),this,SLOT(tagChooceDoneSot(QString,QPoint)));

    connect(ui->actionClear,SIGNAL(triggered()),this,SLOT(actSlot()));
    connect(ui->actionMerge,SIGNAL(triggered()),this,SLOT(actSlot()));
    connect(ui->actionDelete,SIGNAL(triggered()),this,SLOT(actSlot()));
    connect(ui->actionDetect,SIGNAL(triggered()),this,SLOT(actSlot()));
    connect(ui->actionDivide,SIGNAL(triggered()),this,SLOT(actSlot()));
    connect(ui->actionEdit,SIGNAL(triggered()),this,SLOT(actSlot()));
    connect(ui->actionLoadSave,SIGNAL(triggered()),this,SLOT(actSlot()));
    connect(ui->actionDetectFill,SIGNAL(triggered()),this,SLOT(actSlot()));

    connect(ui->startListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(listSelChangedSlot()));
    connect(ui->endListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(listSelChangedSlot()));
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),this,SLOT(sliderMovedSlot(int)));
    connect(ui->editSlider,SIGNAL(sliderMoved(int)),this,SLOT(editSliderMovedSlot(int)));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(openFileSlot()));
    connect(ui->generateButton,SIGNAL(clicked()),this,SLOT(openToolWidget()));
    connect(ui->tagEditButton,SIGNAL(clicked()),this,SLOT(openToolWidget()));
    connect(ui->formatButton,SIGNAL(clicked()),this,SLOT(openToolWidget()));
    connect(ui->splitButton,SIGNAL(clicked()),this,SLOT(splitBtnSlot()));

    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(posChangeSlot(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(durChangedSlot(qint64)));

    connect(ui->tagListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(listDoubleClickedSlot(QListWidgetItem *)));
    connect(ui->startListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(listDoubleClickedSlot(QListWidgetItem *)));
    connect(ui->endListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(listDoubleClickedSlot(QListWidgetItem *)));
    connect(ui->startListWidget->verticalScrollBar(),SIGNAL(valueChanged(int )),this,SLOT(scrollCangedSlot(int)));
    connect(ui->tagListWidget->verticalScrollBar(),SIGNAL(valueChanged(int )),this,SLOT(scrollCangedSlot(int)));
    connect(ui->endListWidget->verticalScrollBar(),SIGNAL(valueChanged(int )),this,SLOT(scrollCangedSlot(int)));

    connect(mySplitThread,SIGNAL(finished()),this,SLOT(splitFinished()));
    connect(mySplitThread,SIGNAL(progress(int)),this,SLOT(splitProgress(int)));
    connect(myDetectThread,SIGNAL(standOutput(QString)),this,SLOT(detectFinishedSlot(QString)));
}
void splitWidget::openFileSlot()
{
    if(currentState != currentState::idle)
        return;
    fileName ="";
    fileName = QFileDialog::getOpenFileName(this,"Open Video", "F:/", tr("video Files (*.mp4 *.avi)"));
    if(fileName.isEmpty())
        return;
    player->setVideoOutput(videoWidget);

    player->setMedia(QUrl::fromLocalFile(fileName));

    player->setNotifyInterval(10);
    ui->openButton->setText(fileName);
    videoPos = 0;
    player->pause();
}
void splitWidget::durChangedSlot(qint64 newDuration)
{
    VideoDuration = newDuration;
    ui->horizontalSlider->setRange(0,VideoDuration);
    ui->progressLabel->setText("0/"+QString::number(VideoDuration/1000.0,'f',2)+" s");
}
void splitWidget::playBtnSlot()
{
    if(!player->isVideoAvailable())
        return;
    if(player->state() == QMediaPlayer::PlayingState)
        player->pause();
    else if(player->state() == QMediaPlayer::PausedState || player->state() == QMediaPlayer::StoppedState)
        player->play();
}
void splitWidget::posChangeSlot(qint64 newPos)
{
    videoPos = newPos;
    ui->horizontalSlider->setValue(newPos);
    QString positionStr,durationStr;
    positionStr.setNum(videoPos/1000.0,'f',2);
    durationStr.setNum(VideoDuration/1000.0,'0',2);
    ui->progressLabel->setText(positionStr+"/"+durationStr+" s");
    if(videoPos >= VideoDuration)
        playBtnSlot();
    for(int i = 0;i<ui->startListWidget->count();i++)
    {
        double start = ui->startListWidget->item(i)->text().toDouble();
        double end = ui->endListWidget->item(i)->text().toDouble();
        if(end == 0)
            end = VideoDuration/1000.0;
        if(newPos/1000.0 >= start && newPos/1000.0 < end)
            ui->tagListWidget->setItemSelected(ui->tagListWidget->item(i),true);
        else
            ui->tagListWidget->setItemSelected(ui->tagListWidget->item(i),false);
    }
}
void splitWidget::sliderMovedSlot(int newValue)
{
    if(!player->isVideoAvailable())
        return;
    videoPos = newValue;
    if(player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
        player->setPosition(videoPos);
        player->play();
    }
    else if(player->state() == QMediaPlayer::PausedState)
        player->setPosition(videoPos);
}
void splitWidget::editSliderMovedSlot(int newValue)
{
    if(ui->startListWidget->selectedItems().isEmpty() && ui->endListWidget->selectedItems().isEmpty())
        return;
    QString positionStr;
    positionStr.setNum(newValue/1000.0,'f',2);
    if(!ui->startListWidget->selectedItems().isEmpty())
        ui->startListWidget->selectedItems().at(0)->setText(positionStr);
    else if(!ui->endListWidget->selectedItems().isEmpty())
        ui->endListWidget->selectedItems().at(0)->setText(positionStr);
    autoSave();
    if(!player->isVideoAvailable())
        return;
    videoPos = newValue;
    player->pause();
    player->setPosition(videoPos);

}
void splitWidget::markBtnSLot()
{

    if(currentState != currentState::idle)
        return;
    QString positionStr;
    positionStr.setNum(videoPos/1000.0,'f',2);
    bool isStartSel = !ui->startListWidget->selectedItems().isEmpty();
    bool isEndSel = !ui->endListWidget->selectedItems().isEmpty();
    if(!isStartSel && !isEndSel)
    {

        ui->startListWidget->addItem(positionStr);
        ui->endListWidget->addItem(QString::number(VideoDuration/1000.0,'f',2));
        ui->tagListWidget->addItem("");
        ui->endListWidget->setItemSelected(ui->endListWidget->item(ui->endListWidget->count()-1),true);
    }
    else if(isStartSel && !isEndSel)
    {
        ui->startListWidget->selectedItems().at(0)->setText(positionStr);
        ui->startListWidget->setCurrentItem(ui->startListWidget->selectedItems().at(0));
        int currentRow = ui->startListWidget->currentRow();
        ui->startListWidget->setItemSelected(ui->startListWidget->selectedItems().at(0),false);
        ui->endListWidget->setItemSelected(ui->endListWidget->item(currentRow),true);
    }
    else if(!isStartSel && isEndSel)
    {
        ui->endListWidget->setCurrentItem(ui->endListWidget->selectedItems().at(0));
        int currentRow = ui->endListWidget->currentRow();
        if(currentRow < ui->endListWidget->count()-1)
        {
            ui->endListWidget->currentItem()->setText(positionStr);
        }
        else if(currentRow == ui->endListWidget->count()-1)
        {
            ui->endListWidget->currentItem()->setText(positionStr);
            ui->startListWidget->addItem(positionStr);
            ui->endListWidget->addItem(QString::number(VideoDuration/1000.0,'f',2));
            ui->tagListWidget->addItem("");
            ui->endListWidget->setItemSelected(ui->endListWidget->currentItem(),false);
            ui->endListWidget->setItemSelected(ui->endListWidget->item(ui->endListWidget->count()-1),true);
        }
    }
    listItemFlagSetup();
    autoSave();
}
void splitWidget::splitBtnSlot()
{
    if(!player->isVideoAvailable() || ui->startListWidget->count() == 0)
        return;
    saveDir = "";
    saveDir = QFileDialog::getExistingDirectory(this,"output directory","F:/");
    if(saveDir.isEmpty())
        return;
    startNodeList.clear();
    endNodeList.clear();
    tagNodeList.clear();
    for(int i = 0;i < ui->startListWidget->count() ; ++i)
    {
        startNodeList.append(ui->startListWidget->item(i)->text().toDouble());
        endNodeList.append(ui->endListWidget->item(i)->text().toDouble());
        tagNodeList.append(ui->tagListWidget->item(i)->text());
    }
    totalCountStr = QString::number(startNodeList.count());
    ui->splitButton->setText("0/"+totalCountStr);
    currentState = currentState::spliting;
    mySplitThread->threadInit(startNodeList,endNodeList,tagNodeList,saveDir,fileName);
    splitProgressDialog->dialogInit(startNodeList.count());
}
void splitWidget::splitFinished()
{
    ui->splitButton->setText("split");
    currentState = currentState::idle;
    splitProgressDialog->done(1);
}
void splitWidget::splitProgress(int progress)
{
    ui->splitButton->setText(QString::number(progress)+"/"+totalCountStr);
    if(splitProgressDialog !=nullptr)
        splitProgressDialog->setProgressValue(progress);
}
void splitWidget::actSlot()
{
    if(currentState != currentState::idle)
        return;
    QAction *senderAct = qobject_cast<QAction*>(this->sender());
    if(onContextWidget == nullptr)
        return;
    int currentRow = -1;
    if(!onContextWidget->selectedItems().isEmpty())
    {
        onContextWidget->setCurrentItem(onContextWidget->selectedItems().at(0));
        currentRow = onContextWidget->currentRow();
    }
    if(senderAct == ui->actionDelete) //删除
    {
        if(currentRow == -1)
            return;
        delete ui->startListWidget->takeItem(currentRow);
        delete ui->endListWidget->takeItem(currentRow);
        delete ui->tagListWidget->takeItem(currentRow);
        onContextWidget->item(onContextWidget->currentRow())->setSelected(false);
    }
    else if(senderAct == ui->actionClear)//清空
    {
        ui->startListWidget->clear();
        ui->endListWidget->clear();
        ui->tagListWidget->clear();
    }
    else if(senderAct == ui->actionMerge)//合并
    {
        if(currentRow == ui->startListWidget->count()-1 || currentRow == -1)
            return;
        ui->endListWidget->item(currentRow)->setText(ui->endListWidget->item(currentRow+1)->text());
        delete ui->startListWidget->takeItem(currentRow+1);
        delete ui->endListWidget->takeItem(currentRow+1);
        delete ui->tagListWidget->takeItem(currentRow+1);
    }
    else if(senderAct == ui->actionDetect)//矫正当前已有剪切节点
    {
        detectMode = detectMode::correct;
        fragmentDetect();
    }
    else if (senderAct == ui->actionDivide)//分裂
    {
        if(currentRow == -1)
            return;
        double temp = videoPos/1000.0;
        double start = ui->startListWidget->item(currentRow)->text().toDouble();
        double end = ui->endListWidget->item(currentRow)->text().toDouble();
        if(temp <= start || temp >= end)
            temp = (start+end)/2;
        ui->startListWidget->insertItem(currentRow+1,QString::number(temp,'f',2));
        ui->endListWidget->insertItem(currentRow+1,ui->endListWidget->item(currentRow)->text());
        ui->tagListWidget->insertItem(currentRow+1,"");
        ui->endListWidget->item(currentRow)->setText(QString::number(temp,'f',2));
        listItemFlagSetup();
    }
    else if(senderAct == ui->actionEdit)//编辑
    {
        if(currentRow == -1)
            return;
        onContextWidget->editItem(onContextWidget->item(currentRow));
    }
    else if(senderAct == ui->actionLoadSave)//加载自动存档文件
        loadSaveFile();
    else if(senderAct == ui->actionDetectFill)//自动检测视频转场点,并生成剪切节点
    {
        detectMode = detectMode::generate;
        fragmentDetect();
    }
    if(senderAct != nullptr && senderAct != ui->actionLoadSave)
        autoSave();
}
void splitWidget::fragmentDetect()
{
    if(!player->isVideoAvailable())
        return;
    QString  program = QCoreApplication::applicationDirPath()+"/sceneDetect.exe";
    QStringList args;
    args<<fileName;
    myDetectThread->threadInit(program,args,-1);
    ui->splitButton->setText("detecting");
    ui->splitButton->setDisabled(true);
    currentState = currentState::detectting;
}
void splitWidget::detectFinishedSlot(QString output)
{
    ui->splitButton->setText("split");
    ui->splitButton->setDisabled(false);
    currentState = currentState::idle;
    QString sceneStr = output;
    if(sceneStr == "")
        return;
    QList<double> startList;
    QList<double> endList;
    int began = 0;
    int end = 0;
    double offset = 0;//向内偏移的时间长度
    for(int i = 0 ;i < sceneStr.count(",");i++)//向startList与endList写入数据
    {
        end = sceneStr.indexOf("\r\n",began+1);
        QString temp;
        if(began == 0)
            temp = sceneStr.mid(began,end-began);
        else
            temp = sceneStr.mid(began+1,end-began-1);
        int pos = temp.indexOf(",");
        startList.append(temp.mid(0,pos).toDouble()+offset);
        endList.append(temp.mid(pos+1,temp.length()).toDouble()-offset);
        began = end;
    }
    if(detectMode == detectMode::correct)
        for(int i = 0 ; i < ui->startListWidget->count() ; i++)//匹配矫正startListWidget与endListWidget中的数据
        {
            double startNum = numberMatch(ui->startListWidget->item(i)->text().toDouble(),startList);
            double endNum = numberMatch(ui->endListWidget->item(i)->text().toDouble(),endList);
            ui->startListWidget->item(i)->setText(QString::number(startNum,'f',2));
            ui->endListWidget->item(i)->setText(QString::number(endNum,'f',2));
        }
    else if(detectMode == detectMode::generate)//向ui中listWidget写入数据
    {
        for(int i = 0 ; i < startList.count() ; i++)
        {
            ui->startListWidget->addItem(QString::number(startList.at(i),'f',2));
            ui->endListWidget->addItem(QString::number(endList.at(i),'f',2));
            ui->tagListWidget->addItem("");
        }
        listItemFlagSetup();
    }
    autoSave();
}
void splitWidget::resizeEvent(QResizeEvent *event)
{
    videoWidget->resize(ui->playWidget->size());
}
void splitWidget::listSelChangedSlot()
{
    QListWidget *listWidget = qobject_cast<QListWidget*>(this->sender());
    int scope = 5000;
    int temp = 500,mini  = 0,max = 1000;
    if(listWidget == ui->startListWidget && !ui->startListWidget->selectedItems().isEmpty())
    {
        int currentRow = listWidget->row(listWidget->selectedItems().at(0));
        int border = ui->endListWidget->item(currentRow)->text().toDouble()*1000;
        temp = listWidget->selectedItems().at(0)->text().toDouble()*1000;
        mini = temp-scope <= 0 ? 0 : temp-scope;
        max = temp+scope >= border ? border : temp+scope;
        if(!ui->endListWidget->selectedItems().isEmpty())
            ui->endListWidget->setItemSelected(ui->endListWidget->selectedItems().at(0),false);
    }
    else if(listWidget == ui->endListWidget && !ui->endListWidget->selectedItems().isEmpty())
    {
        int currentRow = listWidget->row(listWidget->selectedItems().at(0));
        int border = ui->startListWidget->item(currentRow)->text().toDouble()*1000;
        temp = listWidget->selectedItems().at(0)->text().toDouble()*1000;
        mini = temp-scope <= border ? border : temp-scope;
        max = temp+scope >= VideoDuration ? VideoDuration : temp+scope;
        if(!ui->startListWidget->selectedItems().isEmpty())
            ui->startListWidget->setItemSelected(ui->startListWidget->selectedItems().at(0),false);
    }
    ui->editSlider->setMinimum(mini);
    ui->editSlider->setMaximum(max);
    ui->editSlider->setValue(temp);
}
void splitWidget::listDoubleClickedSlot(QListWidgetItem *item)
{
    if(!player->isVideoAvailable())
        return;
    int itemRow = item->listWidget()->row(item);
    double startNum = ui->startListWidget->item(itemRow)->text().toDouble();
    double endNum = ui->endListWidget->item(itemRow)->text().toDouble();
    QString objName = this->sender()->objectName();
    if(objName == "tagListWidget")
    {
        if(videoPos < startNum*1000 || videoPos > endNum*1000)
            videoPlay(startNum);
    }
    else if(objName == "startListWidget")
        videoPlay(startNum);
    else if(objName == "endListWidget")
        videoPlay(endNum - 1);
}
void splitWidget::videoPlay(double pos)//pos以秒为单位
{
    if(!player->isVideoAvailable())
        return;
    pos = pos > 0 ? pos:0;
    pos = pos < VideoDuration/1000.0 ? pos:VideoDuration/1000.0;
    videoPos = pos*1000;
    player->setPosition(videoPos);
    ui->horizontalSlider->setValue(videoPos);
    player->play();
}
void splitWidget::mousePressEvent(QMouseEvent *event)
{
    if(ui->playWidget->geometry().contains(event->pos()))
    {
        if(event->button() == Qt::LeftButton)
            this->playBtnSlot();
        else if(event->button() == Qt::RightButton)
            this->markBtnSLot();
    }
}
bool splitWidget::eventFilter(QObject *obj, QEvent *event)
{
    QListWidget *list = qobject_cast<QListWidget*>(obj);
    if(list == ui->tagListWidget && event->type() == QEvent::ContextMenu)
    {
        QPoint pos = ui->tagListWidget->mapFromGlobal(QCursor::pos());
        QListWidgetItem *item = ui->tagListWidget->itemAt(pos);
        if(ui->tagListWidget->row(item) != -1)
        {
            myTagDialog->hide();
            myTagDialog->show();
        }
    }
    else if(list == ui->startListWidget && event->type() == QEvent::ChildPolished)
        onContextWidget = ui->startListWidget;
    else if(list == ui->endListWidget && event->type() == QEvent::ChildPolished)
        onContextWidget = ui->endListWidget;

    return false;
}
void splitWidget::tagChooceDoneSot(QString tag,QPoint pos)
{
    QListWidgetItem *item = ui->tagListWidget->itemAt(ui->tagListWidget->mapFromGlobal(pos));
    if(ui->tagListWidget->row(item) != -1)
        item->setText(item->text()+tag);
    autoSave();
}
void splitWidget::scrollCangedSlot(int value)
{
    ui->startListWidget->verticalScrollBar()->setValue(value);
    ui->tagListWidget->verticalScrollBar()->setValue(value);
    ui->endListWidget->verticalScrollBar()->setValue(value);
}
double splitWidget::numberMatch(double number,QList<double> data)
{
    if(data.isEmpty())
        return  number;
    int matchIndex = 0;
    double distance = number,distanceOld = number;
    for(int i = 0 ; i < data.count() ; i++)
    {
        distance = qAbs(number - data.at(i));
        if(distance < distanceOld)
            matchIndex = i;
        distanceOld = distance;
    }
    return data.at(matchIndex);
}
void splitWidget::openToolWidget()
{
    if(currentState != currentState::idle)
        return;
    QString objName = this->sender()->objectName();
    if(objName == "generateButton")
    {
        generateWidget *myWidget = new class generateWidget(this);
        myWidget->show();
    }
    else if(objName == "tagEditButton")
    {
        tagEditWidget *myWidget = new class tagEditWidget(this);
        myWidget->show();
    }
    else if(objName == "formatButton")
    {
        formatCovertWidget *myWidget = new class formatCovertWidget(this);
        myWidget->show();
    }
}
void splitWidget::debugeOut(QString debugInfo)
{
    ui->debugLabel->setText(debugInfo);
    QTimer::singleShot(3000,ui->debugLabel,SLOT(clear()));
}
void splitWidget::listItemFlagSetup()
{
    QFlags<Qt::ItemFlag> flags = Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsUserCheckable;
    for(int i = 0 ;i <ui->startListWidget->count();i++)
    {
        ui->startListWidget->item(i)->setFlags(flags);
        ui->endListWidget->item(i)->setFlags(flags);
        ui->tagListWidget->item(i)->setFlags(flags);
    }
}
void splitWidget::loadSaveFile()
{
    QFile saveFile(QCoreApplication::applicationDirPath()+"/autosave.txt");
    QString dataStr = "";
    if(saveFile.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        dataStr = saveFile.readAll();
        saveFile.close();
    }
    int began = 0,end = 0;
    for(int  i = 0 ; i < dataStr.count("\n") ; i++)
    {
        end = dataStr.indexOf("\n",began+1);
        QString temp;
        if(began == 0)
            temp = dataStr.mid(began,end-began);
        else
            temp = dataStr.mid(began+1,end-began-1);
        ui->startListWidget->addItem(temp.section(",",0,0));
        ui->endListWidget->addItem(temp.section(",",1,1));
        ui->tagListWidget->addItem(temp.section(",",2,2));
        began = end;

    }
    listItemFlagSetup();
}
void splitWidget::autoSave()
{
    QFile saveFile(QCoreApplication::applicationDirPath()+"/autosave.txt");
    if(!saveFile.open(QIODevice::Text | QIODevice::QIODevice::WriteOnly))
        return;
    QString dataStr = "";
    for (int i = 0;i<ui->startListWidget->count();i++)
    {
        dataStr.append(ui->startListWidget->item(i)->text());
        dataStr.append(",");
        dataStr.append(ui->endListWidget->item(i)->text());
        dataStr.append(",");
        dataStr.append(ui->tagListWidget->item(i)->text());
        dataStr.append("\n");
    }
    saveFile.write(dataStr.toUtf8());
    saveFile.close();
}
