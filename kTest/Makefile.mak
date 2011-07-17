#CPATH = ..\..\..\bin\Dev-cpp
#CC = $(CPATH)\bin\g++.exe
#INC =  -I$(CPATH)\include 
#LIB =  -L$(CPATH)\lib
#all:
#	$(CC) -c *.cpp $(INC) $(DEF) -DWIN32
#	$(LB) ..\kgmEngine.lib -+ *.obj


CPATH = ..\..\..\bin\Dev-cpp
CC = $(CPATH)\bin\g++.exe
AR = $(CPATH)\bin\ar.exe
DEFS = -DWIN32
#FLGS = -ansi
DIRS = -I$(CPATH)\include -L$(CPATH)\lib -L..\\ -L..\kgmMain\lib

LIBS = -lopengl32 -lopenal32 -lkgmEngine

all:
	$(CC) -g -o kTest.exe *.cpp $(DEFS) $(DIRS) $(LIBS)
	
	

