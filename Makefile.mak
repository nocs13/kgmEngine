CPATH = ..\..\..\bin\Dev-cpp
CC = $(CPATH)\bin\g++.exe
AR = $(CPATH)\bin\ar.exe
DEFS = -DWIN32
#FLGS = -ansi
DIRS = -I$(CPATH)\include -I.\ -I.\inc -I..\kgmMath -L$(CPATH)\lib
LIBS = -lGL -lopenal -lkgmMath -lpthread

all:
	echo compiling...
	make -C kgmMath -f Makefile.mak
	make -C kgmMain -f Makefile.mak
	make -C kgmGraphics -f Makefile.mak
	make -C kgmPhysics -f Makefile.mak
	make -C kgmScript -f Makefile.mak
	make -C kgmAI -f Makefile.mak
	make -C kgmGame -f Makefile.mak

	echo linking...
	ar crs libkgmEngine.lib kgmMath/*.o
	ar crs libkgmEngine.lib kgmMain/*.o
	ar crs libkgmEngine.lib kgmScript/*.o
	ar crs libkgmEngine.lib kgmAI/*.o
	ar crs libkgmEngine.lib kgmPhysics/*.o
	ar crs libkgmEngine.lib kgmGraphics/*.o
	ar crs libkgmEngine.lib kgmGame/*.o

clean:
	echo "clear all..."
	rm -f kgmMath/*.o
	rm -f kgmMain/*.o
	rm -f kgmGraphics/*.o
	rm -f kgmPhysics/*.o
	rm -f kgmScript/*.o
	rm -f kgmAI/*.o
	rm -f kgmGame/*.o
	rm -f libkgmEngine.a

kgmMath:
	make -C kgmMath -f Makefile
	ar crs libkgmEngine.lib kgmMath/*.o

kgmMain:	
	make -C kgmMain -f Makefile
	ar crs libkgmEngine.lib kgmMain/*.o

kgmGraphics:
	make -C kgmGraphics -f Makefile
	ar crs libkgmEngine.lib kgmAI/*.o

kgmPhysics:
	make -C kgmPhysics -f Makefile
	ar crs libkgmEngine.lib kgmPhysics/*.o

kgmAI:
	make -C kgmAI -f Makefile
	ar crs libkgmEngine.lib kgmGraphics/*.o

kgmGame:
	make -C kgmGame -f Makefile
	ar crs libkgmEngine.lib kgmGame/*.o

kgmScript:
	make -C kgmScript -f Makefile
	ar crs libkgmEngine.lib kgmScript/*.o
