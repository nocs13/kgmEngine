# Compilation Tools
CC  := $(ANDROID_NDK_BIN)/arm-linux-androideabi-gcc
CPP := $(ANDROID_NDK_BIN)/arm-linux-androideabi-g++
AR  := $(ANDROID_NDK_BIN)/arm-linux-androideabi-ar

# Compiler flags
# Specify all the flags below which you want to use for your compilation, For this simple example, we only need to specify the include directory path
CFLAGS          := -I$(NDK_ROOT)/platforms/android-9/arch-arm/usr/include
CFLAGS          += -DANDROID -DOSL -DGLES_1 -fpermissive -g #-Werror  

subdirs := $(wildcard ../kgm*/)
sources := $(wildcard $(addsuffix *.cpp,$(subdirs)))
objects := $(patsubst %.cpp,%.o,$(sources))

OUT = libkgmEngine.a

all: $(OUT)

$(objects) : %.o : %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(OUT): $(objects)
	$(AR) -crs $(OUT) $(objects)

sdk: $(OUT)
	cp $(OUT) ../libkgmEngine_android.a

clean:
	rm -f $(objects)
	rm -f $(OUT)
