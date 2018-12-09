#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T15:25:34
#
#-------------------------------------------------

QT       += core gui widgets
QT += network opengl
INCLUDEPATH += .

TARGET = BCIshootingGame
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    Bullet.cpp \
    Enemy.cpp \
    Game.cpp \
    Health.cpp \
    Player.cpp \
    Score.cpp \
    mindwaveconnector.cpp \
    redbox.cpp \
    bluebox.cpp \
    bciscore.cpp \
    poorscore.cpp

HEADERS += \
    Bullet.h \
    Enemy.h \
    Game.h \
    Health.h \
    Player.h \
    Score.h \
    mindwaveconnector.h \
    client.h \
    redbox.h \
    bluebox.h \
    bciscore.h \
    poorscore.h
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../MindwaveSDK3.2/StreamSDKforPC/libs/x64/ -lthinkgear64
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../MindwaveSDK3.2/StreamSDKforPC/libs/x64/ -lthinkgear64d

INCLUDEPATH += $$PWD/../../MindwaveSDK3.2/StreamSDKforPC/libs/x64
DEPENDPATH += $$PWD/../../MindwaveSDK3.2/StreamSDKforPC/libs/x64

unix:!macx|win32: LIBS += -L$$PWD/../../MindwaveSDK3.2/StreamSDKforPC/libs/x64/ -lthinkgear64

INCLUDEPATH += $$PWD/../../MindwaveSDK3.2/StreamSDKforPC/libs/x64
DEPENDPATH += $$PWD/../../MindwaveSDK3.2/StreamSDKforPC/libs/x64
