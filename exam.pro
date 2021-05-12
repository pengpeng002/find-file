#-------------------------------------------------
#
# Project created by QtCreator 2021-03-24T11:20:56
#
#-------------------------------------------------

QT       += core gui axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = exam
TEMPLATE = app
CONFIG -= console
RC_ICONS = Rewrite.ico
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        testclass.cpp \
    reject.cpp \
    get_all_file.cpp \
    search_in_tree.cpp \
    watcher.cpp \
    setting.cpp \
    imagewidget.cpp

HEADERS += \
        testclass.h \
    ui_testclass.h \
    reject.h \
    get_all_file.h \
    search_in_tree.h \
    watcher.h \
    setting.h \
    imagewidget.h

RESOURCES += \
    image.qrc
