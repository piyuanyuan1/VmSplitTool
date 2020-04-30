#include "spiltWidget/splitwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    splitWidget w;
    w.show();
    return a.exec();
}
