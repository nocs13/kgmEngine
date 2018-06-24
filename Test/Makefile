OS = $(shell uname -s)

RM =

CC = ccache g++
EXT =
DEFS =
FLGS = -ansi -std=gnu++11
DIRS =	-I./ -I../kgmBase -I../kgmMath -I../kgmGame -I../kgmSystem -I../kgmSystem/inc -I../kgmPhysics \
	-I../kgmGraphics -L../kgmSystem/lib -L../
LIBS = -llua -llualib

ifeq ($(OS), Linux)
	DEFS = -DLINUX -DDEBUG -DTEST -DALSA -DGPUSHADERS -DEDITOR -DVULKAN
	FLGS = -g -fPIC -O0 -ansi -std=gnu++11
	LIBS +=  -lpthread -lX11 -lGL -ldl
	RM = rm -f
else
	CC = g++
	EXT = .exe
	DEFS = -DWIN32 -DDEBUG -DTEST -DGPUSHADERS -DDSOUND -DEDITOR
	FLGS = -m32 -g -O0 -std=gnu++11 #-mwindows -static-libgcc -static-libstdc++
	LIBS += -lopengl32 -luser32 -lkernel32 -lgdi32 -ldsound -lWinmm -llua
	RM = del
endif

all:
#	make -C ..
#	$(CC) -g -c $(DEFS) $(FLAGS) *.cpp
	$(CC) -g -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
#	$(CC) -g -c $(DEFS) $(INCS) $(LIBD) $(libs) kTest.cpp
#	$(CC) -g -o kTest $(DEFS) $(FLAGS) $(OBJS) $(INCS) $(libs) -L../ -L../kgmLua
	$(CC) -g -o kTest$(EXT) *.o ../libkgmEngine.a $(DEFS) $(FLGS) $(DIRS) $(LIBS)

clean:
	$(RM) kTest$(EXT) *.o

run:
	./kTest$(EXT)

android: clean
	make -C android
