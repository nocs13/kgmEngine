OS = $(shell uname -s)
RM =

CC = g++
DEFS = -DLINUX
FLGS = -ansi -std=c++17
DIRS =	-I./ -I../../kgmBase -I../../kgmMath -I../../kgmGame -I../../kgmSystem -I../../kgmSystem/inc -I../../kgmPhysics \
	-I../../kgmGraphics -L../../kgmSystem/lib -L../../
LIBS = -lpthread -lXxf86vm -lGL -lGLU -lopenal

ifeq ($(OS), Linux)
	DEFS = -std=c++17 -DLINUX -DDEBUG -DTEST -DOAL -DGPUSHADERS
	FLGS = -g -fPIC -O0 -fpermissive  -ansi -std=gnu++11
	LIBS = -ldl -lpthread -lXxf86vm -lX11 -lGL -lGLU -llua -llualib
	RM = rm -f
else
	DEFS = -DWIN32 -DDEBUG -DTEST -DGPUSHADERS
	FLGS = -g -O0 -mwindows -fpermissive -std=gnu++11
	LIBS = -lpthread -lopengl32 -luser32 -lkernel32 -lgdi32 -lglu32 -ldsound -llua -llualib -lWinmm
	RM = rm -f
endif

all:
	$(CC) -g -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
	$(CC) -o kEditor *.o ../../libkgmEngine_s.a $(DEFS) $(FLGS) $(DIRS) $(LIBS)

clean:
	$(RM) -f kEditor *.o
