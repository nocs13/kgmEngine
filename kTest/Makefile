CC = g++
DEFS = -DLINUX
#FLGS = -ansi -std=c++98 -fno-access-control -fno-elide-constructors -ffor-scope -fms-extensions
DIRS = -I./ -I../kgmMath -I../kgmMain -I../kgmGame -I../kgmSystem/inc -L../ -L../kgmSystem/lib	
LIBS = -lGL -lXxf86vm -lpthread -lopenal -lkgmEngine -llua #-lkgmMath -lkgmMain -lkgmPhysics -lkgmGraphics -lkgmAI -lkgmGame
#  -llua5.1
#OBJS =  ./*.o		\
#../kgmMain/*.o		\
#../kgmGame/*.o		\
#../kgmAI/*.o		\
#../kgmGraphics/*.o	\
#../kgmPhysics/*.o


all:
#	$(CC) -g -c $(DEFS) $(FLAGS) *.cpp
	$(CC) -g -c *.cpp $(DEFS) $(DIRS) $(LIBS)
#	$(CC) -g -c $(DEFS) $(INCS) $(LIBD) $(libs) kTest.cpp
#	$(CC) -g -o kTest $(DEFS) $(FLAGS) $(OBJS) $(INCS) $(libs) -L../ -L../kgmLua
#	$(CC) -g -o kTest $(DEFS) $(FLAGS) $(INCS) $(LIBD) $(libs)
	$(CC) -o kTest *.o $(DIRS) $(LIBS)
#	cp    kTest ../kTest.bin

