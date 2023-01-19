CC = clang++


all: clean
	$(CC) -o Game.bin main.c *.mm ../libkgmEngine_s.a -lobjc -framework AppKit -framework Foundation -framework Cocoa -framework Metal -framework MetalKit

clean:
	rm -f Game.bin
