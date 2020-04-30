QT       += core gui
QT       += multimedia
QT       += multimediawidgets
QT += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    formatCovertWidget/formatcovertwidget.cpp \
    public/filenameinfo.cpp \
    generateWidget/generatewidget.cpp \
    main.cpp \
    public/outerprogramthread.cpp \
    spiltWidget/splitProgressDialog/splitprogressdialog.cpp \
    spiltWidget/splitthread.cpp \
    spiltWidget/splitwidget.cpp \
    tagChooceWidget/tagchoocewidget.cpp \
    tagEditWidget/renamethread.cpp \
    tagEditWidget/tageditwidget.cpp

HEADERS += \
    formatCovertWidget/formatcovertwidget.h \
    public/filenameinfo.h \
    generateWidget/generatewidget.h \
    public/outerprogramthread.h \
    spiltWidget/splitProgressDialog/splitprogressdialog.h \
    spiltWidget/splitthread.h \
    spiltWidget/splitwidget.h \
    tagChooceWidget/tagchoocewidget.h \
    tagEditWidget/renamethread.h \
    tagEditWidget/tageditwidget.h

FORMS += \
    formatCovertWidget/formatcovertwidget.ui \
    generateWidget/generatewidget.ui \
    spiltWidget/splitProgressDialog/splitprogressdialog.ui \
    spiltWidget/splitwidget.ui \
    tagChooceWidget/tagchoocewidget.ui \
    tagEditWidget/tageditwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
RC_ICONS = resource/image/icon.ico
