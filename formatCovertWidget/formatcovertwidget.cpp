#include "formatcovertwidget.h"
#include "ui_formatcovertwidget.h"

formatCovertWidget::formatCovertWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::formatCovertWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::WindowCloseButtonHint| Qt::Window);
    fileName = "";
    ffmpegThread = new class outerProgramThread();
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(openBtnSlot()));
    connect(ui->convertButton,SIGNAL(clicked()),this,SLOT(converBtnSlot()));
    connect(ffmpegThread,SIGNAL(threadFinished()),this,SLOT(converFinishedSlot()));
}

formatCovertWidget::~formatCovertWidget()
{
    delete ui;
    delete ffmpegThread;
}
void formatCovertWidget::openBtnSlot()
{
    fileName = QFileDialog::getOpenFileName(this,"选择文件","F:/",tr("video Files (*.mp4 *.avi *.flv *.mkv *.rmvb)"));
    ui->lineEdit->setText(fileName);
    ui->label->setText(getOffExtension(fileName));
}
void formatCovertWidget::converBtnSlot()
{
    if(!QFile::exists(fileName))
        return;
    QStringList args;
    args<<"-i"<<fileName<<getOffExtension(fileName)+ui->comboBox->currentText()<<"-y";
    QString ffmpeg = QCoreApplication::applicationDirPath()+"/ffmpeg/bin/ffmpeg.exe";
    ffmpegThread->threadInit(ffmpeg,args);
    ui->convertButton->setText("converting");
    ui->convertButton->setDisabled(true);
}
void formatCovertWidget::converFinishedSlot()
{
    ui->convertButton->setText("convert");
    ui->convertButton->setDisabled(false);
}
QString formatCovertWidget::getOffExtension(QString filename)
{
    int count = filename.count(".");
    int startPos = 0;
    int endPos = 0;
    for (int i = 0;i<count;i++)
    {
        endPos = filename.indexOf(".",startPos);
        startPos = endPos+1;
    }
    return filename.mid(0,endPos);
}
