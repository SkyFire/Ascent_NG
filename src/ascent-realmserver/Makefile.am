INCLUDES += -I$(srcdir) -I$(srcdir)/../ascent-shared

bin_PROGRAMS = ascent-realmserver

ascent_realmserver_SOURCES = \
CharacterHandler.cpp \
ClientManager.cpp \
ClientManager.h \
ClusterManager.cpp \
ClusterManager.h \
LogonCommClient.h \
LogonCommClient.cpp \
LogonCommHandler.cpp \
LogonCommHandler.h \
Master.cpp \
Master.h \
ObjectStorage.cpp \
ObjectStorage.h \
QueryHandler.cpp \
RStdAfx.cpp \
RStdAfx.h \
Session.cpp \
Session.h \
Structures.h \
WorkerOpcodes.h \
WorkerServer.cpp \
WorkerServer.h \
WorkerServerSocket.cpp \
WorkerServerSocket.h \
WorldSocket.cpp \
WorldSocket.h
	
ascent_realmserver_LDADD = -lshared -lz

ascent_realmserver_LDFLAGS = -L$(srcdir)/../ascent-shared \
               -L/usr/lib \
               @LIB_EXPORT@
