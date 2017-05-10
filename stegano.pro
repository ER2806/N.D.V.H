QT += core
QT += gui

CONFIG += c++11

TARGET = ctegano
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
INCLUDEPATH += /usr/local/include

macx {
    LIBS += /usr/local/lib/libjpeg.a
}

linux-g++ {
    LIBS += /usr/lib/x86_64-linux-gnu/libjpeg.a
}

win32 {
# Add path to libjpeg yourself! :)
}

SOURCES += main.cpp \
    base_encoder.cpp \
    base_decoder.cpp \
    utils.cpp \
    mp3_src/mp3_decoder.cpp \
    mp3_src/mp3_encoder.cpp \
    mp3_src/raw_head.cpp \
    wav_src/wav_decoder.cpp \
    wav_src/wav_encoder.cpp \
    mp3_src/head.cpp \
    image_src/bmp_decoder.cpp \
    image_src/jpg_decoder.cpp \
    image_src/bmp_encoder.cpp \
    image_src/jpg_encoder.cpp




HEADERS += \
    base_encoder.h \
    base_decoder.h \
    formats.h \
    utils.h \
    base_exception.h \
    mp3_src/base_head.h \
    mp3_src/head.h \
    mp3_src/io.h \
    mp3_src/mp3_decoder.h \
    mp3_src/mp3_encoder.h \
    mp3_src/raw_head.h \
    wav_src/wav_decoder.h \
    wav_src/wav_encoder.h \
    image_src/bmp_decoder.h \
    image_src/jpg_decoder.h \
    image_src/bmp_encoder.h \
    image_src/jpg_encoder.h
