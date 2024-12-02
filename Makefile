-include Makefile.mac

subdirs := $(wildcard kgm*/)
subdirs += $(wildcard kgmGraphics/render)
subdirs += $(wildcard kgmGame/actors)
subdirs += $(wildcard kgmGame/objects)
subdirs += $(wildcard kgmScript/libscript)
sources := $(wildcard $(addsuffix *.cpp,$(subdirs)))
sources += $(wildcard kgmGraphics/render/*.cpp)
sources += $(wildcard kgmGame/actions/*.cpp)
sources += $(wildcard kgmGame/objects/*.cpp)
sources += $(wildcard kgmGame/editor/*.cpp)
sources += $(wildcard kgmScript/libscript/*.cpp)
headers := $(wildcard $(addsuffix *.h,$(subdirs)))
headers += $(wildcard kgmGraphics/render/*.h)
headers += $(wildcard kgmGame/actions/*.h)
headers += $(wildcard kgmGame/objects/*.h)
headers += $(wildcard kgmGame/editor/*.h)

objects := $(patsubst %.cpp,%.o,$(sources))
#objects := $(patsubst %.cpp,%.o,$(sources),$(headers))

OUT_SO = libkgmEngine.so
OUT_A  = libkgmEngine_s.a

all: extern debug
#	echo $(subdirs)
#	echo $(sources)

edit: all
	make -C Editor
	cd Editor && ./Editor

extern:
	make -C kgmExtern

debug: set_debug $(OUT_SO) $(OUT_A)
#debug: set_debug $(OUT_A)
	echo 'debug finished.'

set_debug:
	$(eval DEFS += -DDEBUG)
	$(eval FLGS += -g)

release: set_release  $(OUT_SO)
	echo 'release finished.'

set_release:
	$(eval DEFS += -DRELEASE)

$(objects) : %.o : %.cpp
	$(CC) $(FLGS) $(DEFS) -c $< -o $@ $(DIRS)

$(OUT_A): $(objects)
	$(AR) -r -c -s $(OUT_A) $(objects)

$(OUT_SO): $(OUT_A)
	$(CC) -shared -o $(OUT_SO) -Wl,--whole-archive $(OUT_A) -Wl,--no-whole-archive $(FLGS) $(DEFS) $(DIRS) $(LIBS)

#$(OUT_SO): $(objects)
#	$(CC) -shared -o $(OUT_SO) $(objects) $(FLGS) $(DEFS) $(DIRS) $(LIBS)

android:
	echo 'Start android library build.'
	make -C android

macos:
	make -C MacOS

sdk: $(OUT_A)  $(OUT_SO)
	mkdir -p sdk
	cp $(OUT_A)  sdk/.
	cp $(OUT_SO) sdk/.

clean:
	make -C kgmExtern clean
	$(RM) $(objects)
	$(RM) $(OUT_SO) $(OUT_A)

run:
	make -C Komble run
