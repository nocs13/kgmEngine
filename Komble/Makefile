# Makefile.common
#TOP ?= $(shell pwd)
#COMPONENT_DIR := $(TOP)
#COMPONENT_INC := $(COMPONENT_DIR)
#COMPONENT_LIB := $(COMPONENT_DIR)
# other_component/Makefile
#TOP ?= ..
#include ../Makefile.mac
#-include $(shell pwd)/../Makefile.mac

include ../Makefile.mac

#OS = $(shell uname -s)

#RM =

#CC = ccache g++
#CC = clang++
#EXT =
#DEFS =
#FLGS = -ansi -std=c++11
DIRS =	-I./ -I../kgmBase -I../kgmMath -I../kgmGame -I../kgmSystem -I../kgmSystem/inc -I../kgmPhysics \
	-I../kgmGraphics -L../kgmSystem/lib -L../
#LIBS = -llua -llualib
#VSH =

#ifeq ($(OS), Linux)
#	DEFS = -DLINUX -DDEBUG -DTEST -DALSA -DGPUSHADERS -DVULKAN
#	FLGS = -g -fPIC -O0 -ansi -std=c++11
#	LIBS +=  -lpthread -lX11 -lGL -ldl
#	RM = rm -f
#        VHS = Data/shaders/nix
#else
#	CC = g++
#	EXT = .exe
#	DEFS = -DWIN32 -DDEBUG -DTEST -DGPUSHADERS -DDSOUND -DVULKAN
#	FLGS = -g -O0 -std=c++11 #-mwindows -static-libgcc -static-libstdc++
#	LIBS += -lopengl32 -luser32 -lkernel32 -lgdi32 -ldsound -lWinmm -llua -llualib -lwsock32
#	RM = del
#        VHS = Data/shaders/win
#endif

all: shaders ../libkgmEngine_s.a
	$(CC) -g -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
	$(CC) -g -o Komble$(EXT) *.o ../libkgmEngine_s.a $(DEFS) $(FLGS) $(DIRS) $(LIBS)

clean:
	make -C .. distclean
	$(RM) Komble$(EXT) *.o

run:
	./Komble$(EXT)

android: clean
	make -C android

shaders:
	./Data/shaders/glsl2spirv.sh

../libkgmEngine_s.a:
	make -C ..
