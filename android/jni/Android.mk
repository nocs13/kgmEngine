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

subdirs := $(wildcard ../kgm*/)

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../kgmBase/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../kgmMath/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../kgmMedia/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../kgmGraphics/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../kgmPhysics/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../kgmSystem/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../kgmGame/*.cpp)


include $(CLEAR_VARS)
	LOCAL_MODULE := libkgmEngine 
	#LOCAL_SRC_FILES := ../libkgmEngine.a
	#LOCAL_C_INCLUDES := ../../kgmBase/
	#LOCAL_SRC_FILES := ../../kgmBase/*.cpp #$(wildcard ../../kgmBase/*.cpp) 
	#LOCAL_SRC_FILES += $(wildcard ../../kgmMath/*.cpp) 
	#LOCAL_SRC_FILES += $(wildcard ../../kgmGraphics/*.cpp) 
	#LOCAL_SRC_FILES += $(wildcard ../../kgmGame/*.cpp) 
	#LOCAL_SRC_FILES += $(wildcard ../../kgmMedia/*.cpp) 
	#LOCAL_SRC_FILES += $(wildcard ../../kgmSystem/*.cpp) 
	#LOCAL_SRC_FILES += $(wildcard ../../kgmPhysics/*.cpp) 

	#LOCAL_SRC_FILES := $(wildcard $(addsuffix *.cpp,$(subdirs)))
	#LOCAL_OBJ_FILES := $(wildcard $(addsuffix *.o,$(subdirs)))
	#LOCAL_SRC_FILES := ../../kgmBase/kgmFile.cpp
	LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
	
	LOCAL_CFLAGS    := -DEBUG -DANDROID -DOSL -Wall -g -Werror
	LOCAL_CFLAGS    += -DGLES_2
	LOCAL_CFLAGS    += -frtti --std=gnu++11
	
	APP_PLATFORM    := android-14
include $(BUILD_STATIC_LIBRARY)


#include $(CLEAR_VARS)
#	LOCAL_MODULE    := libkgmEngine
#	LOCAL_SRC_FILES := ../../Test/kTest.cpp ../../Test/kGui.cpp 
#	LOCAL_C_INCLUDES := ../../../
#	LOCAL_STATIC_LIBRARIES := kgmEngine
	#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
#	LOCAL_LDLIBS    := -llog -ldl -lm -lz -lstdc++ -landroid #-lGLESv2 -lGLESv1_CM
	#LOCAL_LDLIBS    += -lGLESv2
#	LOCAL_LDLIBS    += -lGLESv1_CM
#	LOCAL_LDLIBS    += -lEGL
#	LOCAL_LDLIBS    += -lOpenSLES
#	LOCAL_CFLAGS    := -DANDROID -DOSL -Wall -g #-Werror
	#LOCAL_CFLAGS    += -DGLES_2
#	LOCAL_CFLAGS    += -DGLES_1
#	LOCAL_CFLAGS    += -frtti --std=gnu++11
#include $(BUILD_SHARED_LIBRARY)


