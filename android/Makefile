# Compilation Tools
CC  := $(ANDROID_NDK_BIN)/arm-linux-androideabi-gcc
CPP := $(ANDROID_NDK_BIN)/arm-linux-androideabi-g++
AR  := $(ANDROID_NDK_BIN)/arm-linux-androideabi-ar

# Compiler flags
# Specify all the flags below which you want to use for your compilation, For this simple example, we only need to specify the include directory path
CFLAGS          := -I$(NDK_ROOT)/platforms/android-9/arch-arm/usr/include
CFLAGS          += -DANDROID -DTEST -DOSL -DGLES_2 -fpermissive -g #-Werror

subdirs := $(wildcard ../kgm*/)
sources := $(wildcard $(addsuffix *.cpp,$(subdirs)))
objects := $(patsubst %.cpp,%.o,$(sources))

OUT = libkgmEngine.a

all: $(OUT) lua

$(objects) : %.o : %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(OUT): $(objects)
	$(AR) -crs $(OUT) $(objects)
	$(AR) -r -c -s $(OUT_A) ../kgmExtern/lua-4.0.1/src/*.o
	$(AR) -r -c -s $(OUT_A) ../kgmExtern/lua-4.0.1/src/lib/*.o

lua:
	make -C ../kgmExtern/lua-4.0.1 -f Makefile.android

luaclean:
	make -C ../kgmExtern/lua-4.0.1 clean -f Makefile.android



clean: luaclean
	rm -f $(objects)
	rm -f $(OUT)

release: clean $(OUT)
