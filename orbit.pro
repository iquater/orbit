BUILD_DIR = ../..

CONFIG(debug, debug|release) {
    DESTDIR = $$BUILD_DIR/debug
} else {
    DESTDIR =$$BUILD_DIR/release
}

template = app
target = orbit



HEADERS += orbit.h \

SOURCES += orbit.cpp \

