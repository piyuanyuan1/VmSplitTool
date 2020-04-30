#include "outerprogramthread.h"

outerProgramThread::outerProgramThread(QObject *parent) : QThread(parent)
{

}
void outerProgramThread::run()
{
    process = new QProcess();
    if(QFile::exists(program) || program == "cmd")
    {
        process->start(program,arguments);
        if(process->waitForStarted())
        {
            emit programStarted();
            process->waitForFinished(timeout);
            process->setReadChannel(QProcess::StandardOutput);
            QString output = QString::fromLocal8Bit(process->readAllStandardOutput());
            emit standOutput(output);
            emit programFinished();
        }
    }
    emit threadFinished();
    delete  process;
}
void outerProgramThread::threadInit(QString program,QStringList args,int timeout)
{
    this->program = program;
    this->arguments = args;
    this->timeout = timeout;
    this->start();
}
