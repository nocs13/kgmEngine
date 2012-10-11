CC = g++ -Wall
DEFS = -DLINUX
FLGS = -g -fpermissive -fPIC
DIRS = -L./ -L../kgmSystem/lib 
LIBS =  -lkgmEngine -lopenal -lGLU -lGL -lXxf86vm -lX11 -lpthread -llua5.1
#LIBS = -lkgmEngine 

all:
	rm -f zBuilder *.o
	cp ../libkgmEngine.a .
	$(CC) -o zBuilder $(FLGS) *.cpp $(DEFS) $(DIRS) $(LIBS) 
	rm -f *.o

clean:	
	rm -f zBuilder *.o
