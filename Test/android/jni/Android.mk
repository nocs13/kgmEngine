# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

NDK_TOOLCHAIN_VERSION=4.9

include $(CLEAR_VARS)
	LOCAL_MODULE    := kgmEngine
	LOCAL_SRC_FILES := ../../../android/libkgmEngine.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
	LOCAL_MODULE    := Test
	LOCAL_SRC_FILES := ../../kTest.cpp ../../kGui.cpp
	LOCAL_C_INCLUDES := ../../../
	LOCAL_STATIC_LIBRARIES := kgmEngine
	#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
	LOCAL_LDLIBS    := -llog -ldl -lm -lz -lstdc++ -landroid #-lGLESv2 -lGLESv1_CM
	LOCAL_LDLIBS    += -lGLESv2
#	LOCAL_LDLIBS    += -lGLESv1_CM
	LOCAL_LDLIBS    += -lEGL
	LOCAL_LDLIBS    += -lOpenSLES
	LOCAL_CFLAGS    := -DANDROID -DOSL -fpermissive -std=gnu++11 -Werror -Wall -g #-Werror
	LOCAL_CFLAGS    += -DGLES_2
#	LOCAL_CFLAGS    += -DGLES_1
        LOCAL_CFLAGS    += -frtti -stdlib=libc++ -v
include $(BUILD_SHARED_LIBRARY)


