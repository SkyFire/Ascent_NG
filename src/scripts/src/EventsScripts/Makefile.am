INCLUDES = -I$(top_srcdir)/src/ascent-logonserver \
   -I$(top_srcdir)/src/ascent-realmserver -I$(top_srcdir)/src/ascent-shared -I$(top_srcdir)/src/ascent-voicechat \
   -I$(top_srcdir)/src/ascent-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src -I../Common
INCLUDES += -I$(srcdir)/../../../ascent-shared/g3dlite -I$(srcdir)/../../../ascent-shared/vmap

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSunEventScripts.la

libSunEventScripts_la_SOURCES = Setup.cpp \
../Common/Base.cpp \
Event_DarkPortal.cpp
