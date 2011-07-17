CC  = g++
DEF = -DLINUX
DIR = ""

all:
	echo compiling...
	make -C kgmMath -f Makefile
	make -C kgmMain -f Makefile
	make -C kgmGraphics -f Makefile
	make -C kgmPhysics -f Makefile
	make -C kgmScript -f Makefile
	make -C kgmGame -f Makefile

	echo linking...
	ar crs libkgmEngine.a kgmMath/*.o
	ar crs libkgmEngine.a kgmMain/*.o
	ar crs libkgmEngine.a kgmScript/*.o
	ar crs libkgmEngine.a kgmPhysics/*.o
	ar crs libkgmEngine.a kgmGraphics/*.o
	ar crs libkgmEngine.a kgmGame/*.o

clean:
	echo "clear all..."
	rm -f kgmMath/*.o
	rm -f kgmMain/*.o
	rm -f kgmGraphics/*.o
	rm -f kgmPhysics/*.o
	rm -f kgmScript/*.o
	rm -f kgmGame/*.o
	rm -f libkgmEngine.a

kgmMath:
	make -C kgmMath -f Makefile
	ar crs libkgmEngine.a kgmMath/*.o

kgmMain:	
	make -C kgmMain -f Makefile
	ar crs libkgmEngine.a kgmMain/*.o

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

kgmScript:
	make -C kgmScript -f Makefile
	ar crs libkgmEngine.a kgmScript/*.o



