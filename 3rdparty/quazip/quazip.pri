#############################################################
# QuaZip 1.x Qt6-compatible .pri for qmake
# Include this in your .pro: include(3rdparty/quazip/quazip.pri)
#############################################################

DEFINES += QUAZIP_USE_QT_ZLIB

# Base path to QuaZip folder
QUAZIP_DIR = $$PWD/quazip

# Include paths for headers
INCLUDEPATH += $$QUAZIP_DIR
DEPENDPATH += $$QUAZIP_DIR

# Headers with Q_OBJECT (for moc)
HEADERS += \
    $$QUAZIP_DIR/quazip.h \
    $$QUAZIP_DIR/quazipfile.h \
    $$QUAZIP_DIR/quazipfileinfo.h \
    $$QUAZIP_DIR/quaziodevice.h \
    $$QUAZIP_DIR/quagzipfile.h \
    $$QUAZIP_DIR/quazipnewinfo.h \
    $$QUAZIP_DIR/quazip_qt_compat.h \
    $$QUAZIP_DIR/quazip_textcodec.h

# Source files
SOURCES += \
    $$QUAZIP_DIR/JlCompress.cpp \
    $$QUAZIP_DIR/quazip.cpp \
    $$QUAZIP_DIR/quazipfile.cpp \
    $$QUAZIP_DIR/quazipnewinfo.cpp \
    $$QUAZIP_DIR/quazipfileinfo.cpp \
    $$QUAZIP_DIR/quagzipfile.cpp \
    $$QUAZIP_DIR/quaziodevice.cpp \
    $$QUAZIP_DIR/quacrc32.cpp \
    $$QUAZIP_DIR/quaadler32.cpp \
    $$QUAZIP_DIR/quachecksum32.cpp \
    $$QUAZIP_DIR/qioapi.cpp \
    $$QUAZIP_DIR/unzip.c \
    $$QUAZIP_DIR/zip.c
SOURCES += $$QUAZIP_DIR/quazip_textcodec.cpp

# Link with zlib
LIBS += -lz

# Qt modules required
QT += core
