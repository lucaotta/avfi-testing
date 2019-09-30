QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_teststockportfolio.cpp \
    ../../app/portfolio.cpp \

HEADERS += \
    ../../app/portfolio.h

INCLUDEPATH += \
    ../../app

