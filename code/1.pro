QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    and.cpp \
    circuit.cpp \
    combin.cpp \
    combininfo.cpp \
    component.cpp \
    dcs.cpp \
    dff.cpp \
    dockwidget.cpp \
    ffinfo.cpp \
    flipflop.cpp \
    gate.cpp \
    gateinfo.cpp \
    history.cpp \
    inlabel.cpp \
    innode.cpp \
    jkff.cpp \
    label.cpp \
    main.cpp \
    mainwindow.cpp \
    mouse.cpp \
    nand.cpp \
    node.cpp \
    nor.cpp \
    not.cpp \
    or.cpp \
    outlabel.cpp \
    outnode.cpp \
    rsff.cpp \
    selectbox.cpp \
    seqin.cpp \
    seqininfo.cpp \
    simul.cpp \
    table.cpp \
    text.cpp \
    tff.cpp \
    value.cpp \
    waveform.cpp \
    wire.cpp \
    xor.cpp

HEADERS += \
    and.h \
    circuit.h \
    combin.h \
    combininfo.h \
    component.h \
    dcs.h \
    dff.h \
    dockwidget.h \
    ffinfo.h \
    flipflop.h \
    gate.h \
    gateinfo.h \
    history.h \
    inlabel.h \
    innode.h \
    jkff.h \
    label.h \
    mainwindow.h \
    mouse.h \
    nand.h \
    node.h \
    nor.h \
    not.h \
    or.h \
    outlabel.h \
    outnode.h \
    rsff.h \
    selectbox.h \
    seqin.h \
    seqininfo.h \
    simul.h \
    table.h \
    text.h \
    tff.h \
    value.h \
    waveform.h \
    wire.h \
    xor.h

FORMS += \
    combininfo.ui \
    ffinfo.ui \
    gateinfo.ui \
    mainwindow.ui \
    seqininfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    components/and.png \
    components/dff.png \
    components/jkff.png \
    components/nand.png \
    components/nand/0.png \
    components/nor.png \
    components/not.png \
    components/or.png \
    components/rsff.png \
    components/tff.png \
    components/xor.png
