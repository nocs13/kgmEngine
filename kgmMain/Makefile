CC = g++
DEFS = -DLINUX
#FLGS = -ansi
DIRS = -I./ -I./inc -I../kgmMath
LIBS = -lGL -lopenal -lkgmMath -lpthread
#LIBA = libkgmMain.a
LIBO = libkgmMain.so

all:
	$(CC) -g -c *.cpp $(DEFS) $(DIRS)
	ar crs ../libkgmEngine.a *.o

shared:
	$(CC) -g -c *.cpp $(DEFS) $(DIRS)
	$(CC) -shared -o $(LIBO)  *.o $(DIRS) $(LIBS)
	cp $(LIBO) ../
	
