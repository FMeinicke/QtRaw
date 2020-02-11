include(../common-config.pri)

TARGET = qtraw
TEMPLATE = lib
CONFIG += c++14 \
    link_pkgconfig \
    plugin
DESTDIR = imageformats

build_pass:CONFIG(debug, debug|release) {
    win32: TARGET = $$join(TARGET,,,d) # 'd' suffix for debug builds on Windows
}

unix: {
    PKGCONFIG += \
        libraw
}

HEADERS += \
    datastream.h \
    raw-io-handler.h
SOURCES += \
    datastream.cpp \
    main.cpp \
    raw-io-handler.cpp
OTHER_FILES += \
    raw.json

win32: {
    INCLUDEPATH *= $$PWD/../LibRaw/libraw/

    LIBS += -L$$OUT_PWD/../libs -llibraw
}

target.path += $$[QT_INSTALL_PLUGINS]/imageformats
INSTALLS += target

unix:!isEmpty(INSTALL_KDEDIR): {
    # For KDE, install a .desktop file with metadata about the loader
    kde_desktop.files = raw.desktop
    kde_desktop.path = $${INSTALL_KDEDIR}/share/kde4/services/qimageioplugins/
    INSTALLS += kde_desktop
}
