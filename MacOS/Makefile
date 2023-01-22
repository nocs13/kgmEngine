CC = clang++


all: clean engine
	$(CC) -std=c++11 -DDARWIN -o Game.bin main.c *.mm ../libkgmEngine_s.a -llua -lobjc -L../kgmSystem/lib -framework AppKit -framework Foundation -framework Cocoa -framework Metal -framework MetalKit

engine:
	file ../libkgmEngine_s.a
clean:
	rm -f Game.bin
