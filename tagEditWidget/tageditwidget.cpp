#include "tageditwidget.h"
#include "ui_tageditwidget.h"

tagEditWidget::tagEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tagEditWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::WindowCloseButtonHint |Qt::Window);
    myTagDialog = new class tagChooceWidget();
    renameThread = new class renameThread();
    ui->tagListWidget->installEventFilter(this);
    connect(ui->sourceButton,SIGNAL(clicked()),this,SLOT(openDirSlot()));
    connect(ui->targetButton,SIGNAL(clicked()),this,SLOT(openDirSlot()));
    connect(ui->sourceEdit,SIGNAL(textChanged(const QString)),this,SLOT(sourceChangedSlot(QString)));
    connect(ui->confirmButton,SIGNAL(clicked()),this,SLOT(confirmBtnSlot()));
    connect(myTagDialog,SIGNAL(selDone(QString,QPoint)),this,SLOT(tagChooceDoneSot(QString,QPoint)));
    connect(ui->fileListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(listDoubleClickedSlot(QListWidgetItem *)));
    connect(ui->tagListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(listDoubleClickedSlot(QListWidgetItem *)));
    connect(ui->fileListWidget->verticalScrollBar(),SIGNAL(valueChanged(int )),this,SLOT(scrollCangedSlot(int)));
    connect(ui->tagListWidget->verticalScrollBar(),SIGNAL(valueChanged(int )),this,SLOT(scrollCangedSlot(int)));
    connect(ui->fileListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(listSelChangedSlot()));
    connect(ui->tagListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(listSelChangedSlot()));
    connect(renameThread,SIGNAL(renameFinished()),this,SLOT(renameFinishedSlot()));
    this->move((QApplication::desktop()->width() - this->width()) / 2, (QApplication::desktop()->height() - this->height()) / 2);
}

tagEditWidget::~tagEditWidget()
{
    delete ui;
    delete myTagDialog;
    delete renameThread;
}
void tagEditWidget::confirmBtnSlot()
{
    if(ui->fileListWidget->count() == 0)
        return;
    collectionStr = ui->checkBox->checkState() == Qt::Checked ? "Collection#" : "";

    fileNameList.clear();
    tagList.clear();
    currentCount = 0;
    for (int i = 0;i<ui->fileListWidget->count();i++)
    {
        QString inputName = ui->sourceEdit->text()+"/"+ui->fileListWidget->item(i)->text();
        fileNameList.append(inputName);
        tagList.append(ui->tagListWidget->item(i)->text());
    }
    renameThread->threadInit(fileNameList,tagList,collectionStr,ui->targetEdit->text());
    ui->confirmButton->setText("processing");
    ui->confirmButton->setDisabled(true);
}
void tagEditWidget::renameFinishedSlot()
{
    ui->confirmButton->setText("confirm");
    ui->confirmButton->setDisabled(false);
    sourceChangedSlot(ui->sourceEdit->text());
}

void tagEditWidget::openDirSlot()
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
void tagEditWidget::sourceChangedSlot(QString newPath)
{
    QDir souceDir(newPath);
    if(!souceDir.exists())
        return;
    ui->fileListWidget->clear();
    ui->tagListWidget->clear();
    QStringList nameFilters;
    nameFilters << "*.mp4";
    QStringList files = souceDir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for(int i = 0;i<files.count();i++)
    {
        ui->fileListWidget->addItem(files.at(i));
        QStringList tags = fileNameInfo::getTags(files.at(i));
        QString tagStr = "";
        for(int i = 0; i<tags.count();i++)
        {
            tagStr.append(tags.at(i));
            tagStr.append("#");
        }
        ui->tagListWidget->addItem(tagStr);
        QListWidgetItem *item = ui->tagListWidget->item(ui->tagListWidget->count()-1);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsUserCheckable);
        QListWidgetItem *item1 = ui->fileListWidget->item(ui->fileListWidget->count()-1);
        item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
    }
}
bool tagEditWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(qobject_cast<QListWidget*>(obj) == ui->tagListWidget && event->type() == QEvent::ContextMenu)
    {
        QPoint pos = ui->tagListWidget->mapFromGlobal(QCursor::pos());
        QListWidgetItem *item = ui->tagListWidget->itemAt(pos);
        if(ui->tagListWidget->row(item) != -1)
        {
            myTagDialog->hide();
            myTagDialog->show();
        }
    }
    return false;
}
void tagEditWidget::tagChooceDoneSot(QString tag,QPoint pos)
{
    QListWidgetItem *item = ui->tagListWidget->itemAt(ui->tagListWidget->mapFromGlobal(pos));
    if(ui->tagListWidget->row(item) != -1)
        item->setText(item->text()+tag);
}


void tagEditWidget::listDoubleClickedSlot(QListWidgetItem *item)
{
    QString currentFile = "";
    if(this->sender()->objectName() == "fileListWidget")
        currentFile = ui->sourceEdit->text()+"/"+item->text();
    else if(this->sender()->objectName() == "tagListWidget")
    {
        int row = ui->tagListWidget->row(item);
        QListWidgetItem *temp = ui->fileListWidget->item(row);
        currentFile = ui->sourceEdit->text()+"/"+temp->text();
    }
    if(QFile::exists(currentFile))
        ui->axWidget->dynamicCall("URL",currentFile);
}
void tagEditWidget::scrollCangedSlot(int value)
{
    ui->tagListWidget->verticalScrollBar()->setValue(value);
    ui->fileListWidget->verticalScrollBar()->setValue(value);
}
void tagEditWidget::listSelChangedSlot()
{
    if(qobject_cast<QListWidget*>(this->sender()) == ui->fileListWidget && !ui->fileListWidget->selectedItems().isEmpty())
    {
        if(!ui->tagListWidget->selectedItems().isEmpty())
            ui->tagListWidget->setItemSelected(ui->tagListWidget->selectedItems().at(0),false);
    }
    else if(qobject_cast<QListWidget*>(this->sender()) == ui->tagListWidget && !ui->tagListWidget->selectedItems().isEmpty())
    {
        if(!ui->fileListWidget->selectedItems().isEmpty())
            ui->fileListWidget->setItemSelected(ui->fileListWidget->selectedItems().at(0),false);
    }
}
