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
DIRS = -I$(CPATH)\include -I.\ -I.\inc -I..\kgmMath -L$(CPATH)\lib
LIBS = -lGL -lopenal -lkgmMath -lpthread
#LIBA = libkgmMain.a
LIBO = libkgmMain.so

all:
	$(CC) -g -c *.cpp $(DEFS) $(DIRS)
	ar crs ../libkgmEngine.lib *.o

	

