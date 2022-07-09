include ../../Makefile.mac


all:
	$(CC) -g -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
	$(CC) -o kEditor *.o ../../libkgmEngine_s.a $(DEFS) $(FLGS) $(DIRS) $(LIBS) -L../../kgmSystem/lib

clean:
	$(RM) -f kEditor *.o
