include Makefile.mac

subdirs := $(wildcard kgm*/)
subdirs += $(wildcard kgmGraphics/render)
subdirs += $(wildcard kgmGame/actors)
subdirs += $(wildcard kgmGame/objects)
sources := $(wildcard $(addsuffix *.cpp,$(subdirs)))
sources += $(wildcard kgmGraphics/render/*.cpp)
sources += $(wildcard kgmGame/actions/*.cpp)
sources += $(wildcard kgmGame/objects/*.cpp)
sources += $(wildcard kgmGame/editor/*.cpp)

objects := $(patsubst %.cpp,%.o,$(sources))

OUT_SO = libkgmEngine.so
OUT_A  = libkgmEngine_s.a

all: extern debug 
	echo $(subdirs)
	echo $(sources)
	make -C Test

extern:
	make -C kgmExtern

debug: set_debug $(OUT_SO) $(OUT_A)
	echo 'debug finished.'
	make -C Tools/Packer

set_debug:
	$(eval DEFS += -DDEBUG)
	$(eval FLGS += -g)

release: set_release  $(OUT_SO)
	echo 'release finished.'

set_release:
	$(eval DEFS += -DRELEASE)

$(objects) : %.o : %.cpp %.h
	$(CC) $(FLGS) $(DEFS) -c $< -o $@ $(DIRS)

$(OUT_A): $(objects)
	$(AR) -r -c -s $(OUT_A) $(objects)

#$(OUT_SO): $(OUT_A)
#	$(CC) -shared -o $(OUT_SO) $(OUT_A) $(FLGS) $(DEFS) $(DIRS) $(LIBS)
$(OUT_SO): $(objects)
	$(CC) -shared -o $(OUT_SO) $(objects) $(FLGS) $(DEFS) $(DIRS) $(LIBS)

android_static:
	echo 'Start android static library build'
	make -C android

android: android_static
	echo 'Start android applibuild'
	make -C Test android

sdk: $(OUT_A)  $(OUT_SO)
	mkdir -p sdk
	cp $(OUT_A)  sdk/.
	cp $(OUT_SO) sdk/.

clean:
	make -C kgmExtern clean
	make -C Test clean
	$(RM) $(objects)

distclean: clean
	$(RM) $(OUT_SO) $(OUT_A)

test:
	make -C Test run
