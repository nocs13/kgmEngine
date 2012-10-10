include Makefile.maw

all:
	echo "clear all..."
	del /Q kgmBase\*.o
	del /Q kgmMath\*.o
	del /Q kgmGraphics\*.o
	del /Q kgmPhysics\*.o
	del /Q kgmSystem\*.o
	del /Q kgmGame\*.o
	del /Q libkgmEngine.lib
	
	echo compiling...
	make -C kgmBase -f Makefile.mak
	make -C kgmMath -f Makefile.mak
	make -C kgmSystem -f Makefile.mak
	make -C kgmGraphics -f Makefile.mak
	make -C kgmPhysics -f Makefile.mak
	make -C kgmGame -f Makefile.mak

	echo linking...
	ar crs libkgmEngine.lib kgmBase/*.o
	ar crs libkgmEngine.lib kgmMath/*.o
	ar crs libkgmEngine.lib kgmSystem/*.o
	ar crs libkgmEngine.lib kgmPhysics/*.o
	ar crs libkgmEngine.lib kgmGraphics/*.o
	ar crs libkgmEngine.lib kgmGame/*.o

	
	echo "building"
	$(CC) -shared -o libkgmEngine.dll $(DEFS) $(OBJS) $(DIRS)  $(LIBS) --out-implib,libkgmEngine.dll.a
	

clean:
	echo "clear all..."
	del /Q kgmBase\*.o
	del /Q kgmMath\*.o
	del /Q kgmGraphics\*.o
	del /Q kgmPhysics\*.o
	del /Q kgmSystem\*.o
	del /Q kgmGame\*.o
	del /Q libkgmEngine.lib
