CC = g++
DEFS = -DLINUX
#LIBA = libkgmMath.a
#LIBO = libkgmMath.so

all:
	$(CC) -g -c *.cpp $(DEFS)
	ar crs ../libkgmEngine.a *.o
#	$(CC) -shared -g -o $(LIBO) $(DEFS) $(FLAGS) *.o
#	cp $(LIBO) ../
#	rm -f  $(LIBA)
#	ar rcs $(LIBA) *.o
#	cp $(LIBA) ../
	
