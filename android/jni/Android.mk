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

include $(CLEAR_VARS)

BUILD_FILES := $(wildcard ../../kgmBase/*.cpp)
BUILD_FILES += $(wildcard ../../kgmMath/*.cpp)
BUILD_FILES += $(wildcard ../../kgmNet/*.cpp)
BUILD_FILES += $(wildcard ../../kgmPhysics/*.cpp)
BUILD_FILES += $(wildcard ../../kgmGraphics/*.cpp)
BUILD_FILES += $(wildcard ../../kgmGame/*.cpp)

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../kgm*/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_MODULE    := libkgmEngine
LOCAL_CFLAGS    := -DANDROID -fpermissive #-Werror
#LOCAL_SRC_FILES := $(BUILD_FILES)
LOCAL_LDLIBS    := -llog -lGLESv2 -lm -lz

#../../kgmMath/*.cpp ../../kgmGraphics/*.cpp

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
