CC = clang++


all: clean
	$(CC) -o Game.bin main.c *.mm ../libkgmEngine_s.a -framework Foundation -framework Cocoa

clean:
	rm -f Game.bin
