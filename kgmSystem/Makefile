CC = g++
DEFS = -DLINUX
#FLGS = -ansi
DIRS = -I./ -I./inc -I../kgmMath -I../kgmMain
LIBS = -lGL -lopenal -lkgmMath -lpthread
LIBA = libkgmSystem.a
LIBO = libkgmSystem.so

all:
	$(CC) -g -c *.cpp $(DEFS) $(DIRS)
	ar crs $(LIBA) *.o
	ar crs ../libkgmEngine.a *.o

shared:
	$(CC) -g -c *.cpp $(DEFS) $(DIRS)
	$(CC) -shared -o $(LIBO)  *.o $(DIRS) $(LIBS)
	cp $(LIBO) ../
	
