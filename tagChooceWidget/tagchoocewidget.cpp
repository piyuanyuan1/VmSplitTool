#include "tagchoocewidget.h"
#include "ui_tagchoocewidget.h"

tagChooceWidget::tagChooceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tagChooceWidget)
{
    ui->setupUi(this);
    ui->verticalLayout->setContentsMargins(5,5,5,5);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint | Qt::Window);
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(itemClickedSlot(QListWidgetItem *)));
    currentTag = "";
    for(int i = 0;i<ui->listWidget->count();i++)
        ui->listWidget->item(i)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(Qt::black);
    shadow_effect->setBlurRadius(10);
    this->setGraphicsEffect(shadow_effect);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeoutSlot()));
    timer.start(200);
    loadListFile();
}

tagChooceWidget::~tagChooceWidget()
{
    delete ui;
    delete shadow_effect;
}
void tagChooceWidget::itemClickedSlot(QListWidgetItem *item)
{
    currentTag = item->text()+"#";
    emit selDone(currentTag,this->pos());
    this->hide();
}
QString tagChooceWidget::getCurrentTag()
{
    return currentTag;
}
void tagChooceWidget::showEvent(QShowEvent *event)
{
    this->move(QCursor::pos());
    this->show();
}
void tagChooceWidget::timeoutSlot()
{
    if(!ui->listWidget->isActiveWindow())
        this->hide();
}
void tagChooceWidget::loadListFile()
{
    QFile list(QCoreApplication::applicationDirPath() + "/tagList.txt");
    if(!list.exists())
        if(list.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            list.write("默认标签#");
            list.close();
        }
    QStringList tagList;
    if(list.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString listStr = list.readAll();
        listStr = listStr.remove("\n");
        listStr = listStr.remove("\r");
        tagList = listStr.split('#', QString::SkipEmptyParts);
        list.close();
    }
    ui->listWidget->clear();
    for(int i = 0 ; i < tagList.count() ; i++)
    {
        ui->listWidget->addItem(tagList.at(i));
        ui->listWidget->item(i)->setSizeHint(QSize(130,25));
    }
    this->resize(130+10,ui->listWidget->count()*25+10);
}
