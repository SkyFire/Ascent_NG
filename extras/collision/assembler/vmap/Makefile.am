## Process this file with automake to produce Makefile.in

## Sub-directories to parse

## CPP flags for includes, defines, etc.
AM_CPPFLAGS = $(MYSQL_INCLUDES) $(POSTGRE_INCLUDES) -I$(top_builddir)/src/shared -I$(srcdir) -I$(srcdir)/../../../dep/include -I$(srcdir)/../../framework -I$(srcdir)/../../shared -I$(srcdir)/../../../dep/include/g3dlite

#  All libraries will be convenience libraries. Might be changed to shared
#  later.
noinst_LIBRARIES = libascentvmaps.a

libascentvmaps_a_SOURCES = \
	AABSPTree.h \
	BaseModel.cpp \
	BaseModel.h \
	CoordModelMapping.cpp \
	CoordModelMapping.h \
	DebugCmdLogger.cpp \
	DebugCmdLogger.h \
	IVMapManager.h \
	ManagedModelContainer.cpp \
	ManagedModelContainer.h \
	ModelContainer.cpp \
	ModelContainer.h \
	NodeValueAccess.h \
	RayIntersectionIterator.h \
	ShortBox.h \
	ShortVector.h \
	SubModel.cpp \
	SubModel.h \
	TileAssembler.cpp \
	TileAssembler.h \
	TreeNode.cpp \
	TreeNode.h \
	VMapDefinitions.h \
	VMapFactory.cpp \
	VMapFactory.h \
	VMapManager.cpp \
	VMapManager.h
