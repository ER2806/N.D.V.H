QT += core
QT -= gui

CONFIG += c++11

TARGET = wavcoder
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    base_encoder.cpp \
    base_decoder.cpp \
    format_parser.cpp \
    utils.cpp \
    mp3_src/mp3_decoder.cpp \
    mp3_src/mp3_encoder.cpp \
    mp3_src/raw_head.cpp \
    wav_src/wav_decoder.cpp \
    wav_src/wav_encoder.cpp \
    mp3_src/head.cpp




HEADERS += \
    base_encoder.h \
    base_decoder.h \
    format_parser.h \
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
    wav_src/wav_encoder.h
