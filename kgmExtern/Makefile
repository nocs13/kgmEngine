include ../Makefile.mac
LUA=lua-5.3.6
ZLIB=zlib-1.3

ifeq ($(OS), Linux)
  ARCH=posix
else
  ARCH=mingw
endif

all:
	echo "make lua"
	make -C $(LUA) $(ARCH)
	make -C $(LUA) install
	pwd
	mkdir -p ../kgmSystem/inc/lua
	cp -r $(LUA)/include/*.h ../kgmSystem/inc/lua/.
	cp $(LUA)/lib/liblua.a ../kgmSystem/lib/.
	cd $(ZLIB) && ./configure --static && cd ..
	make -C $(ZLIB)
	mkdir -p ../kgmSystem/inc/zlib
	cp $(ZLIB)/zlib.h ../kgmSystem/inc/zlib/.
	cp $(ZLIB)/libz.a ../kgmSystem/lib/.

clean:
	make -C $(LUA) distclean
	make -C $(ZLIB) distclean
	rm -f $(LUA)/include/*
	rm -f $(LUA)/lib/*
	rm -f ../kgmSystem/inc/lua/*
	rm -f  ../kgmSystem/lib/liblua.a
	rm -f  ../kgmSystem/lib/liblualib.a

