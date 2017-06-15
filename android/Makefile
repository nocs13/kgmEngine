# Compilation Tools
CC  := $(ANDROID_NDK_BIN)/arm-linux-androideabi-gcc
CPP := $(ANDROID_NDK_BIN)/arm-linux-androideabi-g++
AR  := $(ANDROID_NDK_BIN)/arm-linux-androideabi-ar

# Compiler flags
# Specify all the flags below which you want to use for your compilation, For this simple example, we only need to specify the include directory path
CFLAGS  := -I$(ANDROID_NDK)/platforms/android-9/arch-arm/usr/include
CFLAGS  += -DANDROID -DOSL -DGLES_2 -fpermissive -std=gnu++11 -Werror -Wall
SYSROOT = $(ANDROID_NDK)/platforms/android-14/arch-arm

subdirs := $(wildcard ../kgm*/)
sources := $(wildcard $(addsuffix *.cpp,$(subdirs)))
objects := $(patsubst %.cpp,%.o,$(sources))

OUT = libkgmEngine.a

all: #debug
	ndk-build -B

debug: set_debug $(OUT)
	echo 'debug finished.'

set_debug:
	echo "android g++ path is $(CC)"
	$(eval CFLAGS += -DDEBUG)
	$(eval CFLAGS += -g)

release: set_release  $(OUT)
	echo 'release finished.'

set_release:
	$(eval CFLAGS += -DRELEASE)

$(objects) : %.o : %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(OUT): $(objects)
	echo 'compile kgmengine'
	$(AR) -crs $(OUT) $(objects)
	$(CPP) -shared -o libkgmEngine.so $(OUT) --sysroot=$(SYSROOT)
	ant jar

clean:
	rm -f $(objects)
	rm -f $(OUT)
