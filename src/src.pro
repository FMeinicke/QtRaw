include(../common-config.pri)

CONFIG(debug,debug|release) {
    TARGET  = qtrawd
} else {
    TARGET  = qtraw
}

TEMPLATE = lib
CONFIG += \
    link_pkgconfig \
    plugin
DESTDIR = imageformats

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

LIBS += $$PWD/../third-party/zlib/bin/zlib1.dll
LIBS += $$PWD/../third-party/iconv/bin/iconv.dll
LIBS += $$PWD/../third-party/libxml2/bin/libxml2.dll
LIBS += $$PWD/../third-party/libjpeg-turbo/bin/libjpeg-62.dll
LIBS += $$OUT_PWD/../libs/rawspeed.dll $$OUT_PWD/../libs/libraw.dll

target.path += $$[QT_INSTALL_PLUGINS]/imageformats
INSTALLS += target

# For KDE, install a .desktop file with metadata about the loader
kde_desktop.files = raw.desktop
kde_desktop.path = $${INSTALL_KDEDIR}/share/kde4/services/qimageioplugins/
INSTALLS += kde_desktop
