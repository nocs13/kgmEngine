include ../../Makefile.mac


all:
	$(CC) -g -c *.cpp `pkg-config --cflags gtk+-3.0` $(DEFS) $(FLGS) $(DIRS) $(LIBS)
	$(CC) -o kEditor *.o ../../libkgmEngine_s.a $(DEFS) $(FLGS) $(DIRS) $(LIBS) -L../../kgmSystem/lib `pkg-config --cflags --libs gtk+-3.0`

clean:
	$(RM) -f kEditor *.o
