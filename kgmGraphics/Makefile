CC = g++
DEFS = -DLINUX
#FLGS = -ansi
DIRS = -I./ -L../
LIBS = -lkgmMath -lkgmMain
LIBA = libkgmGraphics.a
LIBO = libkgmGraphics.so
#ar -rs $(LIBA) *.o

all:
	$(CC) -g -c *.cpp $(DEFS) $(DIRS)
	ar crs ../libkgmEngine.a *.o
#	$(CC) -shared -g -o $(LIBO) $(DEFS) $(DIRS) $(LIBS) *.o
#	cp $(LIBO) ../
#	rm -f  $(LIBA)
#	ar scr $(LIBA) *.o
#	cp $(LIBA) ../
	
