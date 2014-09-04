OS = $(shell uname -s)
RM =

CC = g++ 
DEFS = -DLINUX
FLGS = -ansi -std=c++98 
DIRS =	-I./ -I../kgmBase -I../kgmMath -I../kgmGame -I../kgmSystem -I../kgmSystem/inc -I../kgmPhysics \
	-I../kgmGraphics -I../kgmExtern/lua-4.0.1/include -L../kgmSystem/lib -L../ 
LIBS = -lpthread -lXxf86vm -lGL -lGLU -lopenal

ifeq ($(OS), Linux)
	DEFS = -DLINUX -DDEBUG -DTEST -DOAL -DGPUSHADERS -DLUA -DEDITOR
	FLGS = -g -fPIC -O0 -fpermissive  -ansi
	LIBS = -lpthread -lXxf86vm -lX11 -lGL -lGLU -lopenal
	RM = rm -f
else
	DEFS = -DWIN32 -DDEBUG -DTEST -DGPUSHADERS -DLUA -DEDITOR
	FLGS = -ansi -m32 -masm=intel -g -O0 -mwindows -static-libgcc -static-libstdc++
	LIBS = -lpthread -lopengl32 -luser32 -lkernel32 -lgdi32 -lglu32 -ldsound
	RM = del
endif

all:
#	make -C ..
#	$(CC) -g -c $(DEFS) $(FLAGS) *.cpp
	$(CC) -g -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
#	$(CC) -g -c $(DEFS) $(INCS) $(LIBD) $(libs) kTest.cpp
#	$(CC) -g -o kTest $(DEFS) $(FLAGS) $(OBJS) $(INCS) $(libs) -L../ -L../kgmLua
	$(CC) -o kTest *.o ../libkgmEngine.a $(DEFS) $(FLGS) $(DIRS) $(LIBS)

clean:	
	$(RM) kTest *.o
	
run: 
	./kTest	
