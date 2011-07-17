CC = g++
DEFS = -DLINUX
#FLGS = -ansi -std=c++98 -fno-access-control -fno-elide-constructors -ffor-scope -fms-extensions
LIBS = -lGLU -lGL -lXxf86vm -lopenal -llua
DIRS = -I./ -I../kgmMain/inc -L../ -L../kgmMain/lib
LIBA = libkgmGame.a
LIBO = libkgmGame.so

all:
	$(CC) -g -c *.cpp $(DEFS) $(DIRS) $(LIBS)
	ar crs ../libkgmEngine.a *.o
#	rm -f  $(LIBO)
#	$(CC) -shared -g -o $(LIBO) $(DEFS) $(FLAGS) *.o
#	cp $(LIBO) ../
#	rm -f  $(LIBA)
#	ar -rs $(LIBA) *.o
#	cp $(LIBA) ../

clean:
	rm -f  *.o

	
	
