OS = $(shell uname -s)

RM =

CC = ccache g++
EXT =
DEFS = -DLINUX
FLGS = -ansi -std=c++11
DIRS =	-I./ -I../kgmBase -I../kgmMath -I../kgmGame -I../kgmSystem -I../kgmSystem/inc -I../kgmPhysics \
	-I../kgmGraphics -I../kgmExtern/lua/include -L../kgmSystem/lib -L../
LIBS = -lpthread -lXxf86vm -lGL -lGLU -lopenal

ifeq ($(OS), Linux)
	DEFS = -DLINUX -DDEBUG -DTEST -DALSA -DGPUSHADERS -DLUA -DEDITOR
	FLGS = -g -fPIC -O0 -ansi
	LIBS = -lpthread -lXxf86vm -lX11 -lGL -lGLU -lopenal -ldl
	RM = rm -f
else
	CC = g++
	EXT = .exe
	DEFS = -DWIN32 -DDEBUG -DTEST -DGPUSHADERS -DDSOUND -DLUA -DEDITOR
	FLGS = -m32 -g -O0 #-mwindows -static-libgcc -static-libstdc++
	LIBS = -lopengl32 -luser32 -lkernel32 -lgdi32 -lglu32 -ldsound
	RM = del
endif

all:
#	make -C ..
#	$(CC) -g -c $(DEFS) $(FLAGS) *.cpp
	$(CC) -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
#	$(CC) -g -c $(DEFS) $(INCS) $(LIBD) $(libs) kTest.cpp
#	$(CC) -g -o kTest $(DEFS) $(FLAGS) $(OBJS) $(INCS) $(libs) -L../ -L../kgmLua
	$(CC) -o kTest$(EXT) *.o ../libkgmEngine.a $(DEFS) $(FLGS) $(DIRS) $(LIBS)

clean:
	$(RM) kTest$(EXT) *.o

run:
	./kTest$(EXT)
