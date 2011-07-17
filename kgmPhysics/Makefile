CC = g++
DEFS = -DLINUX
#FLGS = -ansi
DIRS = -I./ -L./

all:
	$(CC) -g -c *.cpp $(DEFS) $(DIRS)
	ar crs ../libkgmEngine.a *.o
#	$(CC) -shared -g -o $(LIBO) $(DEFS) $(FLAGS) *.o
#	cp $(LIBO) ../
#	ar -rs $(LIBA) *.o
#	cp $(LIBA) ../
	
