CC = g++
DEFS = -DLINUX
#FLAGS = -ansi -std=c++98 -fno-access-control -fno-elide-constructors -ffor-scope -fms-extensions
#LIBS = -L../ -L../kgmLua
#libs =  -lGLU -lGL -lXxf86vm -llua5.1 -lkgmGame -lkgmMain -lkgmRender 
LIBS =  -lGLU -lGL -lXxf86vm -lpthread -lopenal -lkgmEngine -llua
DIRS = -I./ -I../kgmMain/inc -L../ -L../kgmMain/lib

all:
	
	$(CC) -g -c $(DEFS) *.cpp $(DIRS) $(LIBS) 
	$(CC) -g -o kssWW *.o $(DEFS) $(DIRS) $(LIBS) 
	

