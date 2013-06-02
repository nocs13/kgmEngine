CC = g++ 
DEFS = -DLINUX
FLGS = -ansi -std=c++98 
DIRS =	-I./ -I../kgmBase -I../kgmMath -I../kgmGame -I../kgmSystem -I../kgmSystem/inc -I../kgmPhysics \
	-I../kgmGraphics -L../kgmSystem/lib -L../	
LIBS = -lpthread -lXxf86vm -lGL -lGLU -lopenal
#LIBS = -lkgmEngine #-lkgmMath -lkgmBase -lkgmPhysics -lkgmGraphics -lkgmAI -lkgmGame


all:
#	make -C ..
#	$(CC) -g -c $(DEFS) $(FLAGS) *.cpp
	$(CC) -g -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
#	$(CC) -g -c $(DEFS) $(INCS) $(LIBD) $(libs) kTest.cpp
#	$(CC) -g -o kTest $(DEFS) $(FLAGS) $(OBJS) $(INCS) $(libs) -L../ -L../kgmLua
	$(CC) -o kTest *.o ../libkgmEngine.a $(DEFS) $(FLGS) $(DIRS) $(LIBS)

clean:	
	rm -f kTest *.o
	
run: 
	./kTest	
