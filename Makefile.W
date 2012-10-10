CC  = g++.exe
DEFS = -DWIN32 
FLGS = -ansi -g -c 
DIRS = -I./ -I../kgmBase -I../kgmMath -I../kgmGame -I../kgmSystem/inc -I./kgmSystem/inc -I../kgmGraphics -I../kgmPhysics -L$(CPATH)\lib -L./kgmSystem/lib
LIBS = -lpthread -llua5.1.w -lopengl32 -lOpenAL32 -luser32 -lkernel32 -lgdi32
OBJS = *.o

all:
	echo cleaning...
	
	echo compiling...
	$(CC) $(FLGS) kgmBase/*.cpp $(DEFS) $(DIRS)
	$(CC) $(FLGS) kgmMath/*.cpp $(DEFS) $(DIRS)
	$(CC) $(FLGS) kgmSystem/*.cpp $(DEFS) $(DIRS)
	$(CC) $(FLGS) kgmPhysics/*.cpp $(DEFS) $(DIRS)
	$(CC) $(FLGS) kgmGraphics/*.cpp $(DEFS) $(DIRS)
	$(CC) $(FLGS) kgmGame/*.cpp $(DEFS) $(DIRS)
	
	echo linking...
	ar crs libkgmEngine.lib.a $(OBJS)

	
	echo "building"
	$(CC) -shared -o libkgmEngine.dll $(DEFS) $(OBJS) $(DIRS)  $(LIBS) 
	
	echo clearing...
	del /Q *.o
	
clean:
	echo "clear all..."
	del /Q kgmBase\*.o
	del /Q kgmMath\*.o
	del /Q kgmGraphics\*.o
	del /Q kgmPhysics\*.o
	del /Q kgmSystem\*.o
	del /Q kgmGame\*.o
	del /Q libkgmEngine.lib
