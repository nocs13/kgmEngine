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

FILE_LIST       := $(wildcard $(LOCAL_PATH)/../../kgm*/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_MODULE    := libkgmEngine
LOCAL_CFLAGS    := -DANDROID -DGLES -fpermissive -Wall -g -std=c99 #-Werror

include $(BUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := kgm
LOCAL_STATIC_LIBRARIES := kgmEngine 
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS    := -llog -lGLESv2 -ldl -lm -lz -lstdc++
include $(BUILD_SHARED_LIBRARY)


