OS = $(shell uname -s)
RM = 

ifeq ($(OS), Linux)
	RM = rm
else
	RM = del
endif

all:
	echo "clear all..."
	$(RM) -f kgmBase/*.o
	$(RM) -f kgmMath/*.o
	$(RM) -f kgmGraphics/*.o
	$(RM) -f kgmPhysics/*.o
	$(RM) -f kgmGame/*.o
	$(RM) -f libkgmEngine.a
	$(RM) -f libkgmEngine.so

	echo "compiling..."
	make -C kgmBase -f Makefile
	make -C kgmMath -f Makefile
	make -C kgmSystem -f Makefile
	make -C kgmGraphics -f Makefile
	make -C kgmPhysics -f Makefile
	make -C kgmGame -f Makefile

	echo "linking..."
	ar crs libkgmEngine.a kgmBase/*.o
	ar crs libkgmEngine.a kgmMath/*.o
	ar crs libkgmEngine.a kgmSystem/*.o
	ar crs libkgmEngine.a kgmPhysics/*.o
	ar crs libkgmEngine.a kgmGraphics/*.o
	ar crs libkgmEngine.a kgmGame/*.o

	echo "building"
	$(CC) -shared -o libkgmEngine.so $(DEFS) $(OBJS) $(DIRS) $(LIBS)

	echo "clear..."
	$(RM) -f kgmMath/*.o
	$(RM) -f kgmBase/*.o
	$(RM) -f kgmSystem/*.o
	$(RM) -f kgmGraphics/*.o
	$(RM) -f kgmPhysics/*.o
	$(RM) -f kgmGame/*.o
	$(RM) -f *.o

clean:
	echo "clear all..."
	$(RM) -f kgmBase/*.o
	$(RM) -f kgmMath/*.o
	$(RM) -f kgmSystem/*.o
	$(RM) -f kgmGraphics/*.o
	$(RM) -f kgmPhysics/*.o
	$(RM) -f kgmGame/*.o
	$(RM) -f libkgmEngine.a
	$(RM) -f libkgmEngine.so

kgmBase:	
	make -C kgmBase -f Makefile
	ar crs libkgmEngine.a kgmBase/*.o

kgmMath:
	make -C kgmMath -f Makefile
	ar crs libkgmEngine.a kgmMath/*.o

kgmGraphics:
	make -C kgmGraphics -f Makefile
	ar crs libkgmEngine.a kgmAI/*.o

kgmPhysics:
	make -C kgmPhysics -f Makefile
	ar crs libkgmEngine.a kgmPhysics/*.o

kgmAI:
	make -C kgmAI -f Makefile
	ar crs libkgmEngine.a kgmGraphics/*.o

kgmGame:
	make -C kgmGame -f Makefile
	ar crs libkgmEngine.a kgmGame/*.o




