include Makefile.mac

subdirs := $(wildcard kgm*/)
sources := $(wildcard $(addsuffix *.cpp,$(subdirs)))
objects := $(patsubst %.cpp,%.o,$(sources))

OUT_SO = libkgmEngine.so
OUT_A  = libkgmEngine.a

all: debug
	make -C Test

debug: set_debug $(OUT_SO)
	echo 'debug finished.'
	make -C Tools/Packer

set_debug:
	$(eval DEFS += -DDEBUG)
	$(eval FLGS += -g)

release: set_release  $(OUT_SO)
	echo 'release finished.'
	make -C Tools/Packer

set_release:
	$(eval DEFS += -DRELEASE)

$(objects) : %.o : %.cpp %.h
	$(CC) $(FLGS) $(DEFS) -c $< -o $@ $(DIRS)

$(OUT_A): $(objects) lua
	$(AR) -r -c -s $(OUT_A) $(objects)
#	$(AR) -r -c -s $(OUT_A) kgmExtern/lua-4.0.1/src/*.o
#	$(AR) -r -c -s $(OUT_A) kgmExtern/lua-4.0.1/src/lib/*.o

$(OUT_SO): $(OUT_A)
	$(CC) -shared -o $(OUT_SO) $(OUT_A) $(FLGS) $(DEFS) $(DIRS) $(LIBS)

lua:
	echo 'building lua'
#	make -C kgmExtern/lua-4.0.1

luaclean:
	echo 'cleaning lua'
#	make -C kgmExtern/lua-4.0.1 clean

sdk: $(OUT_A)  $(OUT_SO)
	mkdir -p sdk
	cp $(OUT_A)  sdk/.
	cp $(OUT_SO) sdk/.

clean: luaclean
#	echo "val: $(os)" # use cmd line make os=<some_val> clean
	rm -f $(objects)

distclean: clean
	rm -f $(OUT_SO) $(OUT_A)
