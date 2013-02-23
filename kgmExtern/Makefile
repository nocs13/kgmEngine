include ../Makefile.mac

all:
	make -C lua linux
	
	cp lua/src/lua.h ../kgmSystem/inc/.
	cp lua/src/lualib.h ../kgmSystem/inc/.
	cp lua/src/luaconf.h ../kgmSystem/inc/.
	cp lua/src/lauxlib.h ../kgmSystem/inc/.
	
	cp lua/src/liblua.a ../kgmSystem/lib/.	
	