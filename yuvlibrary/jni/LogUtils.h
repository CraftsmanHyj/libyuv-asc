//在需要引用的地方调用  #include "LogUtils.h"
//PS: include 之后双引号和尖括号的区别
//1、 #include <jni.h>：系统自带的头文件用尖括号引用，编译器会在系统目录下查找
//2、 #include "LogUtils.h"：用户自定义的头文件，编译器会在用户目录下优先查找，找不到在找系统目录
//源博文：https://blog.csdn.net/afei__/article/details/81030373

//调用方法示例：
//LOGD("hello world");
//LOGD("%d", 10);
//LOGD("%s : %d", "num", 20);
//LOGD();

//输出结果：[文件名][方法名][行号]: format... 的形式
//[main.cpp][main][5]: hello world
//[main.cpp][main][6]: 10
//[main.cpp][main][7]: num : 20
//[main.cpp][main][8]:

#include <android/log.h>
#include <string.h>

#ifndef _LOG_UTILS_H_
#define _LOG_UTILS_H_

//如果想要关掉 log，注释掉 #define DEBUG 的定义就行
#define DEBUG // 可以通过 CmakeLists.txt 等方式来定义在这个宏，实现动态打开和关闭LOG

// Windows 和 Linux 这两个宏是在 CMakeLists.txt 通过 ADD_DEFINITIONS 定义的
#ifdef Windows
#define __FILENAME__ (strrchr(__FILE__, '\\') + 1) // Windows下文件目录层级是'\\'
#elif Linux
#define __FILENAME__ (strrchr(__FILE__, '/') + 1) // Linux下文件目录层级是'/'
#else
#define __FILENAME__ (strrchr(__FILE__, '/') + 1) // 默认使用这种方式
#endif

#ifdef DEBUG
#define TAG "JNI"
#define LOGV(format, ...) __android_log_print(ANDROID_LOG_VERBOSE, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGD(format, ...) __android_log_print(ANDROID_LOG_DEBUG, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGI(format, ...) __android_log_print(ANDROID_LOG_INFO, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGW(format, ...) __android_log_print(ANDROID_LOG_WARN, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGE(format, ...) __android_log_print(ANDROID_LOG_ERROR, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define LOGV(format, ...);
#define LOGD(format, ...);
#define LOGI(format, ...);
#define LOGW(format, ...);
#define LOGE(format, ...);
#endif // DEBUG

#endif // _LOG_UTILS_H_