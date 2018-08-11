CC  := $(ANDROID_NDK_BIN)/arm-linux-androideabi-gcc
CPP := $(ANDROID_NDK_BIN)/arm-linux-androideabi-g++
AR  := $(ANDROID_NDK_BIN)/arm-linux-androideabi-ar

CFLAGS := -I$(ANDROID_NDK)/platforms/android-9/arch-arm/usr/include
CFLAGS += -DANDROID -DOSL -DGLES_2 -fpermissive -std=gnu++11 -Werror -Wall -fno-strict-aliasing -fno-common -flto
SYSROOT = $(ANDROID_NDK)/platforms/android-9/arch-arm

all: build

build: 
#	export HAVE_LTO_PLUGIN=0
#	export LD_LIBRAY_PATH=$LD_LIBRARY_PATH:$(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.7/prebuilt/linux-x86/libexec/gcc/arm-linux-androideabi/4.7
	$(CPP) -g -shared $(CFLAGS) --sysroot $(SYSROOT) -o libTest.so ../*.cpp ../../android/libkgmEngine.a  
#	ant -Dbasedir android -f android/build.xml debug
#	make -C ../../android/ clean && make -C ../../android/
	cp ../Data/* ./assets/.
	$(ANDROID_NDK)/ndk-build -B
	ant clean && ant debug

clean:
	make -C ../../android/ clean
	ant clean
	rm -rf obj libs assets/*

ant:
	ant clean && ant debug

run:
	~/Install/android-sdk-linux/platform-tools/adb uninstall com.example.Test
	~/Install/android-sdk-linux/platform-tools/adb install bin/Test-debug.apk

