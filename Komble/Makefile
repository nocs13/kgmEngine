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

DIRS =	-I./ -I../kgmBase -I../kgmMath -I../kgmGame -I../kgmSystem -I../kgmSystem/inc -I../kgmPhysics \
	-I../kgmGraphics -L../kgmSystem/lib -L../

all: shaders
	$(RM) Komble$(EXT) *.o
	$(CC) -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
	$(CC) -o Komble$(EXT) *.o ../libkgmEngine_s.a $(DEFS) $(FLGS) $(DIRS) $(LIBS) -lXext

clean:
	$(RM) Komble$(EXT) *.o

run:
	./Komble$(EXT)

android: clean
	make -C android

macos: ../MacOS/game.bin
	cp ../MacOS/game.bin Komble

../MacOS/game.bin:
	make -C ../MacOS

shaders:
#	./Data/shaders/glsl2spirv.sh
	echo ""
