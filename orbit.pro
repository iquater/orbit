#BUILD_DIR = ../..

template = app
target = orbit

#INCLUDEPATH +=

HEADERS += orbit.h \
  maneuver.h \
  interfaces.h \
  constants.h \
  body.h \
  utilites.h \
  maneuver_scheme.h \

SOURCES += orbit.cpp \
  maneuver.cpp \
  body.cpp \
  utilites.cpp \
  example.cpp \
  maneuver_scheme.cpp \
