
CONFIG +=
QT       += core gui
QT       += opengl
QT       += xml

TARGET = zomedomeetcie
TEMPLATE = app

#la ligne suivante n est a rajouter que sous linux
LIBS += -lGLU

# Input
HEADERS += fenprincipale.h \
           glwidget.h \
           mathema.h \
           objet3d.h \
           voutenidabeille.h \
           voutenidabeille3d.h \
    rectoid.h \
    parametres.h \
    domenidabeille.h \
    domenidabeille3d.h \
    complexes.h \
    zome.h \
    zome3d.h \
    heliyourte.h \
    heliyourte3d.h \
    geodome.h \
    geodome3d.h \
    toileyourte.h
FORMS += fenprincipale.ui
SOURCES += fenprincipale.cpp \
           glwidget.cpp \
           main.cpp \
           mathema.cpp \
           objet3d.cpp \
           voutenidabeille.cpp \
           voutenidabeille3d.cpp \
    rectoid.cpp \
    parametres.cpp \
    domenidabeille.cpp \
    domenidabeille3d.cpp \
    complexes.cpp \
    zome.cpp \
    zome3d.cpp \
    heliyourte.cpp \
    heliyourte3d.cpp \
    geodome.cpp \
    geodome3d.cpp \
    toileyourte.cpp
RESOURCES += tousLesDocs.qrc
RC_FILE += tousLesDocs.rc  
TRANSLATIONS = zomedomeetcie_en.ts

OTHER_FILES += \
    todo.txt \
    procedureAjout3d.txt
