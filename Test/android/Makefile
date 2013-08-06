all: build

build:
	make -C ../../android/ clean && make -C ../../android/
	cp ../Data/* ./assets/.
	$(NDK_ROOT)/ndk-build -B
	ant clean && ant debug

clean:
	make -C ../../android/ clean
	ant clean
	rm -rf obj libs assets/*
run:
	~/Install/android-sdk-linux/platform-tools/adb uninstall com.example.Test
	~/Install/android-sdk-linux/platform-tools/adb install bin/Test-debug.apk

