QT       += core gui sql network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adddialog.cpp \
    main.cpp \
    mainwindow.cpp \
    randomdialog.cpp \
    searchdialog.cpp \
    settingdialog.cpp \
    tabledialog.cpp \
    tablemodel.cpp \
    wordwidget.cpp

HEADERS += \
    adddialog.h \
    mainwindow.h \
    randomdialog.h \
    searchdialog.h \
    settingdialog.h \
    tabledialog.h \
    tablemodel.h \
    wordwidget.h

FORMS += \
    mainwindow.ui \
    randomdialog.ui \
    searchdialog.ui \
    settingdialog.ui \
    tabledialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
  ../../../../下载/1.ico \
  ../../../../下载/1.ico \
  icon.rc

RESOURCES += \
  qss.qrc

RC_ICONS = 1.ico
