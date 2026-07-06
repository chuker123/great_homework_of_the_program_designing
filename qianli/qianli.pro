QT       += core gui widgets multimedia

CONFIG += c++17

TARGET = qianli
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    gamemanager.cpp \
    models/player.cpp \
    models/card.cpp \
    models/enemy.cpp \
    models/item.cpp \
    systems/battlemanager.cpp \
    systems/choicesystem.cpp \
    systems/shopsystem.cpp \
    systems/inventory.cpp \
    systems/savemanager.cpp \
    ui/battlescene.cpp \
    ui/cardwidget.cpp \
    ui/shopwidget.cpp \
    ui/choicewidget.cpp \
    ui/maingamewidget.cpp \
    ui/levelmapwidget.cpp \
    ui/backpackdialog.cpp \
    ui/gamescene.cpp

HEADERS += \
    mainwindow.h \
    gamemanager.h \
    models/player.h \
    models/card.h \
    models/enemy.h \
    models/item.h \
    systems/battlemanager.h \
    systems/choicesystem.h \
    systems/shopsystem.h \
    systems/inventory.h \
    systems/savemanager.h \
    ui/battlescene.h \
    ui/cardwidget.h \
    ui/shopwidget.h \
    ui/choicewidget.h \
    ui/maingamewidget.h \
    ui/levelmapwidget.h \
    ui/backpackdialog.h \
    ui/gamescene.h

RESOURCES += \
    resources.qrc
