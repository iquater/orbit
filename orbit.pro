#BUILD_DIR = ../..

template = app
target = orbit

INCLUDEPATH += orbit_types/ \
  maneuver/ \
  interfaces/ \

HEADERS += orbit_types/orbit.h \
  maneuver/maneuver.h \
  interfaces/interfaces.h \

SOURCES += orbit_types/orbit.cpp \
  maneuver/maneuver.cpp \

