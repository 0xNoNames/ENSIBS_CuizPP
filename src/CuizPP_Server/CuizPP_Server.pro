QT += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
#CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        banquequestions.cpp \
        joueur.cpp \
        main.cpp \
        partie.cpp \
        question.cpp \
        serveur.cpp \
        serveurgui.cpp \
        typeaudio.cpp \
        typechimere.cpp \
        typeimage.cpp \
        typetexte.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    banquequestions.h \
    joueur.h \
    partie.h \
    question.h \
    serveur.h \
    serveurgui.h \
    typeaudio.h \
    typechimere.h \
    typeimage.h \
    typetexte.h

FORMS += \
    serveurgui.ui
