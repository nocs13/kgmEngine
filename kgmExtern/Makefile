include ../Makefile.mac
LUA=lua-5.3.6

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
#	cp $(LUA)/lib/liblualib.a ../kgmSystem/lib/.

clean:
	make -C $(LUA) clean
	rm -f $(LUA)/include/*
	rm -f $(LUA)/lib/*
	rm -f ../kgmSystem/inc/lua/*
	rm -f  ../kgmSystem/lib/liblua.a
	rm -f  ../kgmSystem/lib/liblualib.a

