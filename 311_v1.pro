#-------------------------------------------------
#
# Project created by QtCreator 2012-11-16T10:13:12
#
#-------------------------------------------------

QT  += core gui
QT  += sql
QT  += network
QT  += xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 311_tester
TEMPLATE = app

CONFIG += thread
#CONFIG += extserialport
CONFIG += serialport

include(serialport/qextserialport.pri)

HEADERS += \
    processing/cmeter.h \
    protocol/protocol.h \
    protocol/ctestpoint.h \
    interface/cselwindow.h \
    interface/cmainwindow.h \
    interface/cloginwindow.h \
    interface/database/cdatabasewindow.h \
    interface/test/ctestwindow.h \
    interface/test/ctestsummarywindow.h \
    interface/test/ctestsettingwindow.h \
    interface/test/adjusttest/cadjusttestwindow.h \
    interface/test/fulltest/cfulltestwindow.h \
    interface/test/fulltest/cfulltestselfpowerwindow.h \
    interface/test/fulltest/cfulltestoverviewwindow.h \
    interface/test/fulltest/cfulltestmotorwindow.h \
    interface/test/fulltest/cfulltestmetervoltagewindow.h \
    interface/test/fulltest/cfulltestmemorywindow.h \
    interface/test/fulltest/cfulltestlcdwindow.h \
    interface/test/fulltest/cfulltestlcdbacklightwindow.h \
    interface/test/fulltest/cfulltestkeywindow.h \
    interface/test/fulltest/cfulltestiooutwindow.h \
    interface/test/fulltest/cfulltestioinwindow.h \
    interface/test/fulltest/cfulltestfreqwindow.h \
    interface/test/fulltest/cfulltestcanwindow.h \
    interface/test/fulltest/cfulltestanalogwindow.h \
    interface/test/inittest/cinittestwindow.h \
    interface/test/fulltest/cfulltestledwindow.h \
    interface/test/bootloader/cbootloaderwindow.h \
    database/cdatabase.h \
    capp.h \
    database/cuser.h \
    processing/ctestjob.h \
    processing/cconfigjob.h \
    protocol/c232port.h \
    protocol/cdataframe.h \
    interface/test/adjusttest/cadjusttestwatertemperaturewindow.h \
    interface/test/adjusttest/cadjusttestspeedtablewindow.h \
    interface/test/adjusttest/cadjusttestrotatetablewindow.h \
    interface/test/adjusttest/cadjusttestoverviewwindow.h \
    interface/test/adjusttest/cadjusttestoiltablewindow.h \
    interface/test/adjusttest/cadjusttestanalogwindow.h \
    interface/test/functiontest/cfunctiontestwindow.h \
    interface/test/functiontest/cfunctiontestwatertemperaturewindow.h \
    interface/test/functiontest/cfunctiontestvehiclevoltagewindow.h \
    interface/test/functiontest/cfunctiontestspeedtablewindow.h \
    interface/test/functiontest/cfunctiontestrotatetablewindow.h \
    interface/test/functiontest/cfunctiontestpressurewindow.h \
    interface/test/functiontest/cfunctiontestoverviewwindow.h \
    interface/test/functiontest/cfunctiontestoilpressurewindow.h \
    interface/test/functiontest/cfunctiontestledwindow.h \
    interface/test/functiontest/cfunctiontestinlettemperaturewindow.h \
    interface/test/functiontest/cfunctiontestfueloiltablewindow.h \
    protocol/c485port.h \
    processing/c485thread.h \
    processing/c232thread.h \
    configure/cconfig.h \
    configure/cadjustconfig.h \
    interface/test/functiontest/cfunctiontestsettingwindow.h \
    configure/cmeterconfig.h \
    interface/configure/cconfigwindow.h \
    interface/configure/cconfigchannelwindow.h \
    interface/configure/cconfigchanneltemplete.h \
    interface/configure/cconfigadjustparmswindow.h \
    interface/configure/cconfigadjustmeterappwindow.h \
    interface/ctitlebar.h \
    interface/csystembutton.h \
    interface/cobjectfactory.h

SOURCES += \
    main.cpp \
    processing/cmeter.cpp \
    protocol/ctestpoint.cpp \
    interface/cselwindow.cpp \
    interface/cmainwindow.cpp \
    interface/cloginwindow.cpp \
    interface/database/cdatabasewindow.cpp \
    interface/test/ctestwindow.cpp \
    interface/test/ctestsummarywindow.cpp \
    interface/test/ctestsettingwindow.cpp \
    interface/test/adjusttest/cadjusttestwindow.cpp \
    interface/test/fulltest/cfulltestwindow.cpp \
    interface/test/fulltest/cfulltestselfpowerwindow.cpp \
    interface/test/fulltest/cfulltestoverviewwindow.cpp \
    interface/test/fulltest/cfulltestmetervoltagetest.cpp \
    interface/test/fulltest/cfulltestmemorywindow.cpp \
    interface/test/fulltest/cfulltestledwindow.cpp \
    interface/test/fulltest/cfulltestlcdwindow.cpp \
    interface/test/fulltest/cfulltestlcdbacklightwindow.cpp \
    interface/test/fulltest/cfulltestkeywindow.cpp \
    interface/test/fulltest/cfulltestiooutwindow.cpp \
    interface/test/fulltest/cfulltestioinwindow.cpp \
    interface/test/fulltest/cfulltestfreqwindow.cpp \
    interface/test/fulltest/cfulltestcanwindow.cpp \
    interface/test/fulltest/cfulltestanalogwindow.cpp \
    interface/test/inittest/cinittestwindow.cpp \
    interface/test/bootloader/cbootloaderwindow.cpp \
    database/cdatabase.cpp \
    capp.cpp \
    database/cuser.cpp \
    processing/cconfigjob.cpp \
    processing/ctestjob.cpp \
    protocol/c232port.cpp \
    protocol/cdataframe.cpp \
    protocol/protocol.cpp \
    interface/test/adjusttest/cadjusttestwatertemperaturewindow.cpp \
    interface/test/adjusttest/cadjusttestspeedtablewindow.cpp \
    interface/test/adjusttest/cadjusttestrotatetablewindow.cpp \
    interface/test/adjusttest/cadjusttestoverviewwindow.cpp \
    interface/test/adjusttest/cadjusttestoiltablewindow.cpp \
    interface/test/adjusttest/cadjusttestanalogwindow.cpp \
    interface/test/functiontest/cfunctiontestwindow.cpp \
    interface/test/functiontest/cfunctiontestwatertemperaturewindow.cpp \
    interface/test/functiontest/cfunctiontestvehiclevoltagewindow.cpp \
    interface/test/functiontest/cfunctiontestspeedtablewindow.cpp \
    interface/test/functiontest/cfunctiontestrotatetablewindow.cpp \
    interface/test/functiontest/cfunctiontestpressurewindow.cpp \
    interface/test/functiontest/cfunctiontestoverviewwindow.cpp \
    interface/test/functiontest/cfunctiontestoilpressurewindow.cpp \
    interface/test/functiontest/cfunctiontestledwindow.cpp \
    interface/test/functiontest/cfunctiontestinlettemperaturewindow.cpp \
    interface/test/functiontest/cfunctiontestfueloiltablewindow.cpp \
    protocol/c485port.cpp \
    processing/c485thread.cpp \
    processing/c232thread.cpp \
    configure/cconfig.cpp \
    configure/cadjustconfig.cpp \
    interface/test/functiontest/cfunctiontestsettingwindow.cpp \
    configure/cmeterconfig.cpp \
    interface/configure/cconfigwindow.cpp \
    interface/configure/cconfigchannelwindow.cpp \
    interface/configure/cconfigadjustparmswindow.cpp \
    interface/configure/cconfigadjustmeterappwindow.cpp \
    interface/ctitlebar.cpp \
    interface/csystembutton.cpp \
    interface/test/fulltest/cfulltestmotorwindow.cpp

RESOURCES += \
    res/311_tester.qrc
RC_FILE = \
    res/311_v1.rc
