OS = $(shell uname -s)
RM =

CC = g++
DEFS = -DLINUX
FLGS = -ansi -std=c++98
DIRS =	-I./ -I../kgmBase -I../kgmMath -I../kgmGame -I../kgmSystem -I../kgmSystem/inc -I../kgmPhysics \
	-I../kgmGraphics -L../kgmSystem/lib -L../
LIBS = -lpthread -lXxf86vm -lGL -lGLU -lopenal

ifeq ($(OS), Linux)
	DEFS = -DLINUX -DDEBUG -DTEST -DOAL -DGPUSHADERS
    FLGS = -g -fPIC -O0 -fpermissive  -ansi
	LIBS = -lpthread -lXxf86vm -lX11 -lGL -lGLU -lopenal
	RM = rm -f
else
	DEFS = -DWIN32 -DDEBUG -DTEST -DGPUSHADERS
    FLGS = -g -O0 -mwindows -fpermissive
    LIBS = -lpthread -lopengl32 -luser32 -lkernel32 -lgdi32 -lglu32 -ldsound
	RM = del
endif

all:
	$(CC) -g -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
	$(CC) -o kEditor *.o ../../libkgmEngine.a $(DEFS) $(FLGS) $(DIRS) $(LIBS)

clean:
	$(RM) -f kEditor *.o
