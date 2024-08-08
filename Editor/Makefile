include ../Makefile.mac

DIRS =	-I./ -I../kgmBase -I../kgmMath -I../kgmGame -I../kgmSystem -I../kgmSystem/inc -I../kgmPhysics \
	-I../kgmGraphics -L../kgmSystem/lib -L../

all:
	$(CC) -g -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
	$(CC) -g -o Editor *.o ../libkgmEngine_s.a $(DEFS) $(FLGS) $(DIRS) $(LIBS)

clean:
	$(RM) -f kEditor *.o
