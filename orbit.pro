#BUILD_DIR = ../..

template = app
target = orbit

INCLUDEPATH += orbit_types/ \
  maneuver/ \

HEADERS += orbit_types/orbit.h \
  maneuver/maneuver.h \

SOURCES += orbit_types/orbit.cpp \
  maneuver/maneuver.cpp \

