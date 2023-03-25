include ../../Makefile.mac

all:
	echo "build packer."
	$(CC) -o packer.bin packer.cpp ../../libkgmEngine_s.a -std=gnu++11 -I../../ -L../../ $(DIRS) -lpthread
#	g++ -g -o packer.bin packer.cpp ../../libkgmEngine_s.a -std=gnu++11 -DLINUX -I../.. -L../.. $(LIBS) $(DIRS)
