CC = clang++

BIN = game.bin

all: clean engine
	$(CC) -std=c++11 -DDARWIN -o $(BIN) main.c *.mm ../libkgmEngine_s.a -llua -lobjc -L../kgmSystem/lib -framework AppKit -framework Foundation -framework Cocoa -framework Metal -framework MetalKit

engine:  ../libkgmEngine_s.a
	echo "Engine builded."

../libkgmEngine_s.a:
	make -C ..

clean:
	rm -f $(BIN)
