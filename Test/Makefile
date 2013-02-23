CC = g++ -Wall -pedantic -ansi
DEFS = -DLINUX
#FLGS = -ansi -std=c++98 -fno-access-control -fno-elide-constructors -ffor-scope -fms-extensions
DIRS =	-I./ -I../kgmBase -I../kgmMath -I../kgmGame -I../kgmSystem -I../kgmSystem/inc -I../kgmPhysics \
	-I../kgmGraphics -L../kgmSystem/lib -L../	
LIBS = -lpthread -lXxf86vm -lGL -lGLU -lopenal
#LIBS = -lkgmEngine #-lkgmMath -lkgmBase -lkgmPhysics -lkgmGraphics -lkgmAI -lkgmGame


all:
#	make -C ..
#	$(CC) -g -c $(DEFS) $(FLAGS) *.cpp
	$(CC) -g -c *.cpp $(DEFS) $(DIRS) $(LIBS)
#	$(CC) -g -c $(DEFS) $(INCS) $(LIBD) $(libs) kTest.cpp
#	$(CC) -g -o kTest $(DEFS) $(FLAGS) $(OBJS) $(INCS) $(libs) -L../ -L../kgmLua
	$(CC) -o kTest *.o ../libkgmEngine.a $(DEFS) $(DIRS) $(LIBS)

clean:	
	rm -f kTest *.o
