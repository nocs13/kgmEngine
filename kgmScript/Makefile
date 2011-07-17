CC = g++
DEFS = -DLINUX
#FLGS = -ansi
DIRS = -I./ -I./inc -L./lib
LIBS = -lGL -lopenal -lpthread -llua
#LIBA = libkgmScript.a
LIBO = libkgmScript.so

all:
	$(CC) -g -c *.cpp $(DEFS) $(DIRS)
	ar crs ../libkgmEngine.a *.o

shared:
	$(CC) -g -c *.cpp $(DEFS) $(DIRS)
	$(CC) -shared -o $(LIBO)  *.o $(DIRS) $(LIBS)
	cp $(LIBO) ../
	
