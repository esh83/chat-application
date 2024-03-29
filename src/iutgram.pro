QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addchat.cpp \
    chatpage.cpp \
    main.cpp \
    queries.cpp \
    requesthandler.cpp \
    setupmain.cpp \
    workerchat.cpp \
    workerlist.cpp \
    workerother.cpp

HEADERS += \
    addchat.h \
    chatpage.h \
    config.h \
    queries.h \
    requesthandler.h \
    setupmain.h \
    workerchat.h \
    workerlist.h \
    workerother.h

FORMS += \
    addchat.ui \
    chatpage.ui \
    setupmain.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += source.qrc

QMAKE_CXXFLAGS += -std=gnu++11
