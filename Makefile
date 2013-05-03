include Makefile.mac

all:    clean build

build:
	echo "compiling..."
#	make -C kgmExtern -f Makefile

	make -C kgmBase -f Makefile
	make -C kgmMath -f Makefile
	make -C kgmNet -f Makefile
	make -C kgmSystem -f Makefile
	make -C kgmGraphics -f Makefile
	make -C kgmPhysics -f Makefile
	make -C kgmMedia -f Makefile
	make -C kgmGame -f Makefile

	echo "linking..."
	ar crs libkgmEngine.a kgmBase/*.o
	ar crs libkgmEngine.a kgmMath/*.o
	ar crs libkgmEngine.a kgmNet/*.o
	ar crs libkgmEngine.a kgmSystem/*.o
	ar crs libkgmEngine.a kgmPhysics/*.o
	ar crs libkgmEngine.a kgmGraphics/*.o
	ar crs libkgmEngine.a kgmMedia/*.o
	ar crs libkgmEngine.a kgmGame/*.o

	echo "building"
	$(CC) -shared -o libkgmEngine.so $(DEFS) $(OBJS) $(DIRS) $(LIBS)

	echo "build test"
	make -C Test -f Makefile

clean:
	echo "clear all..."
	$(RM) kgmBase/*.o
	$(RM) kgmMath/*.o
	$(RM) kgmNet/*.o
	$(RM) kgmSystem/*.o
	$(RM) kgmGraphics/*.o
	$(RM) kgmPhysics/*.o
	$(RM) kgmMedia/*.o
	$(RM) kgmGame/*.o
	$(RM) Test/*.o Test/kTest Test/kTest.exe

distclean: clean	
	$(RM) libkgmEngine.a
	$(RM) libkgmEngine.so

run:
	make -C Test -f Makefile run

sdk: clean build
	mkdir -p kgmEngine
	mkdir -p kgmEngine/kgmBase
	mkdir -p kgmEngine/kgmMath
	mkdir -p kgmEngine/kgmSystem
	mkdir -p kgmEngine/kgmPhysics
	mkdir -p kgmEngine/kgmGraphics
	mkdir -p kgmEngine/kgmGame
	mkdir -p kgmEngine/kgmNet
	cp kgmBase/*.h      kgmEngine/kgmBase/.
	cp kgmMath/*.h      kgmEngine/kgmMath/.
	cp kgmSystem/*.h    kgmEngine/kgmSystem/.
	cp kgmPhysics/*.h   kgmEngine/kgmPhysics/.
	cp kgmGraphics/*.h  kgmEngine/kgmGraphics/.
	cp kgmNet/*.h       kgmEngine/kgmNet/.
	cp kgmGame/*.h      kgmEngine/kgmGame/.
	cp libkgmEngine.a   kgmEngine/.

android:
	make -C android clean && make -C android



