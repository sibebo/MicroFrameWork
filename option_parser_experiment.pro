TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.cpp

HEADERS += \
    src/option_parser.h \
    src/application_base.h

OTHER_FILES += \
    README.md \
    LICENSE
