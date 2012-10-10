include Makefile.mac
#CC  = g++
#DEFS = -DLINUX
#DIRS = -I./ -I./kgmMath -I./kgmBase -I./kgmGame -I./kgmSystem/inc -I./kgmGraphics -I./kgmPhysics -L./kgmSystem/lib
#LIBS = -lpthread -lXxf86vm -lX11 -lGL -lGLU -lopenal -llua
#OBJS = kgmBase/*.o kgmMath/*.o kgmSystem/*.o kgmGraphics/*.o kgmPhysics/*.o kgmGame/*.o

all:
	echo "clear all..."
	rm -f kgmBase/*.o
	rm -f kgmMath/*.o
	rm -f kgmGraphics/*.o
	rm -f kgmPhysics/*.o
	rm -f kgmGame/*.o
	rm -f libkgmEngine.a
	rm -f libkgmEngine.so

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
	rm -f kgmMath/*.o
	rm -f kgmBase/*.o
	rm -f kgmSystem/*.o
	rm -f kgmGraphics/*.o
	rm -f kgmPhysics/*.o
	rm -f kgmGame/*.o
	rm -f *.o
	
clean:
	echo "clear all..."
	rm -f kgmBase/*.o
	rm -f kgmMath/*.o
	rm -f kgmSystem/*.o
	rm -f kgmGraphics/*.o
	rm -f kgmPhysics/*.o
	rm -f kgmGame/*.o
	rm -f libkgmEngine.a
	rm -f libkgmEngine.so

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




