include ../Makefile.mac

all:
	echo "make lua"
	make -C lua-4.0.1 
	mkdir -p ../kgmSystem/inc/lua
	cp lua-4.0.1/include/*.h ../kgmSystem/inc/lua/.	
	cp lua-4.0.1/lib/liblua.a ../kgmSystem/lib/.	
	cp lua-4.0.1/lib/liblualib.a ../kgmSystem/lib/.	

clean:
	make -C lua-4.0.1 clean
#	rm -rf ../kgmSystem/inc/lua
	rm -f  ../kgmSystem/lib/liblua.a	
	
