# This is the Android makefile for libyuv for NDK.
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cc

#编译模块需要的源文件
LOCAL_SRC_FILES := \
    source/compare.cc           \
    source/compare_common.cc    \
    source/compare_gcc.cc       \
    source/compare_mmi.cc       \
    source/compare_msa.cc       \
    source/compare_neon.cc      \
    source/compare_neon64.cc    \
    source/compare_win.cc       \
    source/convert.cc           \
    source/convert_argb.cc      \
    source/convert_from.cc      \
    source/convert_from_argb.cc \
    source/convert_to_argb.cc   \
    source/convert_to_i420.cc   \
    source/cpu_id.cc            \
    source/planar_functions.cc  \
    source/rotate.cc            \
    source/rotate_any.cc        \
    source/rotate_argb.cc       \
    source/rotate_common.cc     \
    source/rotate_gcc.cc        \
    source/rotate_mmi.cc        \
    source/rotate_msa.cc        \
    source/rotate_neon.cc       \
    source/rotate_neon64.cc     \
    source/rotate_win.cc        \
    source/row_any.cc           \
    source/row_common.cc        \
    source/row_gcc.cc           \
    source/row_mmi.cc           \
    source/row_msa.cc           \
    source/row_neon.cc          \
    source/row_neon64.cc        \
    source/row_win.cc           \
    source/scale.cc             \
    source/scale_any.cc         \
    source/scale_argb.cc        \
    source/scale_common.cc      \
    source/scale_gcc.cc         \
    source/scale_mmi.cc         \
    source/scale_msa.cc         \
    source/scale_neon.cc        \
    source/scale_neon64.cc      \
    source/scale_uv.cc          \
    source/scale_win.cc         \
    source/video_common.cc

LOCAL_SRC_FILES += \
        yuvutils.c

#common_CFLAGS := -Wall -fexceptions
#LOCAL_CFLAGS += $(common_CFLAGS)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
#LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../libjpeg-turbo-1.5.0
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../libjpeg-turbo-1.5.0/include
LOCAL_SHARED_LIBRARIES := libjpeg-turbo1500

LOCAL_MODULE := libyuv
LOCAL_MODULE_TAGS := optional
LOCAL_LDLIBS := -ljnigraphics -llog  #此变量包含在构建共享库或可执行文件时要使用的其他链接器标志列表.
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib #用于在C++中调用Android的Log方法，打印日志到LogCat

include $(BUILD_SHARED_LIBRARY)
include $(LOCAL_PATH)/libjpeg-turbo-1.5.0/Android.mk