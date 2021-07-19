#-------------------------------------------------
#
# Project created by QtCreator 2021-04-07T19:22:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GridBased_AllHexMesh
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        twseventree.cpp \
        drawable_twseventree.cpp \
        hex_transition_orient_3ref.cpp \
        hex_transition_install_3ref.cpp

HEADERS += \
        mainwindow.h \
        twseventree.h \
        drawable_twseventree.h \
        hex_transition_schemes_3ref.h \
        hex_transition_orient_3ref.h \
        hex_transition_install_3ref.h

FORMS += \
        mainwindow.ui

#My modifications
DEFINES += CINOLIB_USES_OPENGL
DEFINES += CINOLIB_USES_QT
DEFINES += CINOLIB_USES_EXACT_PREDICATES
QT += core opengl
INCLUDEPATH +=/home/tesi/Scrivania/cinolib/include
INCLUDEPATH +=/home/tesi/Scrivania/cinolib/external/eigen
DATA_PATH       = \\\"$$PWD/../Meshes/\\\"
DEFINES        += DATA_PATH=$$DATA_PATH

# just for Linux
unix:!macx {
DEFINES += GL_GLEXT_PROTOTYPES
LIBS    += -lGLU
}

#DEFINES += NDEBUG
