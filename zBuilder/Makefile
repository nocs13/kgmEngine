include ../Makefile.mac

#CC = g++ -Wall
#DEFS = -DLINUX
#FLGS = -g -fpermissive -fPIC
DIRS = -I./ -I../ -I../kgmSystem/inc -L./ -L../ -L../kgmSystem/lib 
LIBS =  -lpthread -lopenal  -lX11 -lXxf86vm -lGLU -lGL -llua -lkgmEngine
#LIBS = -lkgmEngine 

all:
	rm -f zBuilder *.o
#	cp ../libkgmEngine.a .
	$(CC) $(FLGS) *.cpp $(DEFS)
	$(CC) -o zBuilder *.o $(DEFS) $(DIRS) $(LIBS) 
	rm -f *.o

clean:	
	rm -f zBuilder *.o
