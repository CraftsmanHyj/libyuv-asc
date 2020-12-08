#include <jni.h>
#include <string>
#include "libyuv.h"
#include <android/bitmap.h>

//分别用来存储1420，1420缩放，I420旋转和镜像的数据
static jbyte *Src_i420_data;
static jbyte *Src_i420_data_scale;
static jbyte *Src_i420_data_rotate;

JNIEXPORT void JNI_OnUnload(JavaVM *jvm, void *reserved) {
    //进行释放
    free(Src_i420_data);
    free(Src_i420_data_scale);
    free(Src_i420_data_rotate);
}

void scaleI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint dst_width,
               jint dst_height, jint mode) {

    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);
    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);
    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + dst_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + dst_i420_y_size + dst_i420_u_size;

    libyuv::I420Scale((const uint8 *) src_i420_y_data, width,
                      (const uint8 *) src_i420_u_data, width >> 1,
                      (const uint8 *) src_i420_v_data, width >> 1,
                      width, height,
                      (uint8 *) dst_i420_y_data, dst_width,
                      (uint8 *) dst_i420_u_data, dst_width >> 1,
                      (uint8 *) dst_i420_v_data, dst_width >> 1,
                      dst_width, dst_height,
                      (libyuv::FilterMode) mode);
}

void rotateI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint degree) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + src_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + src_i420_y_size + src_i420_u_size;

    //要注意这里的width和height在旋转之后是相反的
    if (degree == libyuv::kRotate90 || degree == libyuv::kRotate270) {
        libyuv::I420Rotate((const uint8 *) src_i420_y_data, width,
                           (const uint8 *) src_i420_u_data, width >> 1,
                           (const uint8 *) src_i420_v_data, width >> 1,
                           (uint8 *) dst_i420_y_data, height,
                           (uint8 *) dst_i420_u_data, height >> 1,
                           (uint8 *) dst_i420_v_data, height >> 1,
                           width, height,
                           (libyuv::RotationMode) degree);
    }
}

void mirrorI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + src_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + src_i420_y_size + src_i420_u_size;

    libyuv::I420Mirror((const uint8 *) src_i420_y_data, width,
                       (const uint8 *) src_i420_u_data, width >> 1,
                       (const uint8 *) src_i420_v_data, width >> 1,
                       (uint8 *) dst_i420_y_data, width,
                       (uint8 *) dst_i420_u_data, width >> 1,
                       (uint8 *) dst_i420_v_data, width >> 1,
                       width, height);
}


void nv21ToI420(jbyte *src_nv21_data, jint width, jint height, jbyte *src_i420_data) {
    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jbyte *src_nv21_y_data = src_nv21_data;
    jbyte *src_nv21_vu_data = src_nv21_data + src_y_size;

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_y_size + src_u_size;


    libyuv::NV21ToI420((const uint8 *) src_nv21_y_data, width,
                       (const uint8 *) src_nv21_vu_data, width,
                       (uint8 *) src_i420_y_data, width,
                       (uint8 *) src_i420_u_data, width >> 1,
                       (uint8 *) src_i420_v_data, width >> 1,
                       width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_init(JNIEnv *env, jclass type, jint width, jint height, jint dst_width,
                                  jint dst_height) {
    Src_i420_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
    Src_i420_data_scale = (jbyte *) malloc(sizeof(jbyte) * dst_width * dst_height * 3 / 2);
    Src_i420_data_rotate = (jbyte *) malloc(sizeof(jbyte) * dst_width * dst_height * 3 / 2);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_compressYUV(JNIEnv *env, jclass type,
                                         jbyteArray src_, jint width,
                                         jint height, jbyteArray dst_,
                                         jint dst_width, jint dst_height,
                                         jint mode, jint degree,
                                         jboolean isMirror) {
    jbyte *Src_data = env->GetByteArrayElements(src_, NULL);
    jbyte *Dst_data = env->GetByteArrayElements(dst_, NULL);
    //nv21转化为i420
    nv21ToI420(Src_data, width, height, Src_i420_data);
    //进行缩放的操作
//    scaleI420(Src_i420_data, width, height, Src_i420_data_scale, dst_width, dst_height, mode);
//    if (isMirror) {
//        //进行旋转的操作
//        rotateI420(Src_i420_data_scale, dst_width, dst_height, Src_i420_data_rotate, degree);
//        //因为旋转的角度都是90和270，那后面的数据width和height是相反的
//        mirrorI420(Src_i420_data_rotate, dst_height, dst_width, Dst_data);
//    } else {
//        rotateI420(Src_i420_data_scale, dst_width, dst_height, Dst_data, degree);
//    }
    env->ReleaseByteArrayElements(dst_, Dst_data, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_cropYUV(JNIEnv *env, jclass type, jbyteArray src_, jint width,
                                     jint height, jbyteArray dst_, jint dst_width, jint dst_height,
                                     jint left, jint top) {
    //裁剪的区域大小不对
    if (left + dst_width > width || top + dst_height > height) {
        return;
    }

    //left和top必须为偶数，否则显示会有问题
    if (left % 2 != 0 || top % 2 != 0) {
        return;
    }

    jint src_length = env->GetArrayLength(src_);
    jbyte *src_i420_data = env->GetByteArrayElements(src_, NULL);
    jbyte *dst_i420_data = env->GetByteArrayElements(dst_, NULL);


    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + dst_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + dst_i420_y_size + dst_i420_u_size;

    libyuv::ConvertToI420((const uint8 *) src_i420_data, src_length,
                          (uint8 *) dst_i420_y_data, dst_width,
                          (uint8 *) dst_i420_u_data, dst_width >> 1,
                          (uint8 *) dst_i420_v_data, dst_width >> 1,
                          left, top,
                          width, height,
                          dst_width, dst_height,
                          libyuv::kRotate0, libyuv::FOURCC_I420);

    env->ReleaseByteArrayElements(dst_, dst_i420_data, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_yuvI420ToNV21(JNIEnv *env, jclass type, jbyteArray i420Src,
                                           jbyteArray nv21Src,
                                           jint width, jint height) {

    jbyte *src_i420_data = env->GetByteArrayElements(i420Src, NULL);
    jbyte *src_nv21_data = env->GetByteArrayElements(nv21Src, NULL);

    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_y_size + src_u_size;

    jbyte *src_nv21_y_data = src_nv21_data;
    jbyte *src_nv21_vu_data = src_nv21_data + src_y_size;


    libyuv::I420ToNV21(
            (const uint8 *) src_i420_y_data, width,
            (const uint8 *) src_i420_u_data, width >> 1,
            (const uint8 *) src_i420_v_data, width >> 1,
            (uint8 *) src_nv21_y_data, width,
            (uint8 *) src_nv21_vu_data, width,
            width, height);
}

//
// Created by Sharry Choo on 2019-07-24.
//

void NV21ToI420(jbyte *src, jbyte *dst, int width, int height) {
    // NV21 参数
    jint src_y_size = width * height;
    jbyte *src_y = src;
    jbyte *src_vu = src + src_y_size;
    // I420 参数
    jint dst_y_size = width * height;
    jint dst_u_size = dst_y_size >> 2;
    jbyte *dst_y = dst;
    jbyte *dst_u = dst + dst_y_size;
    jbyte *dst_v = dst + dst_y_size + dst_u_size;
    /**
    * <pre>
    * int NV21ToI420(const uint8_t* src_y,
    *          int src_stride_y,
    *          const uint8_t* src_vu,
    *          int src_stride_vu,
    *          uint8_t* dst_y,
    *          int dst_stride_y,
    *          uint8_t* dst_u,
    *          int dst_stride_u,
    *          uint8_t* dst_v,
    *          int dst_stride_v,
    *          int width,
    *          int height);
    * </pre>
    * <p>
    * stride 为颜色分量的跨距: 它描述一行像素中, 该颜色分量所占的 byte 数目, YUV 每个通道均为 1byte(8bit)
    * <p>
    * stride_y: Y 是最全的, 一行中有 width 个像素, 也就有 width 个 Y
    * stride_u: YUV420 的采样为 Y:U:V = 4:1:1, 从整体的存储来看, 一个 Y 分量的数目为 U/V 的四倍
    * 但从一行上来看, width 个 Y, 它会用到 width/2 个 U
    * stride_v: 同 stride_u 的分析方式
    */
    libyuv::NV21ToI420(
            (uint8_t *) src_y, width,
            (uint8_t *) src_vu, width,
            (uint8_t *) dst_y, width,
            (uint8_t *) dst_u, width >> 1,
            (uint8_t *) dst_v, width >> 1,
            width, height
    );
}

void I420ToNV12(jbyte *src, jbyte *dst, int width, int height) {

    jint src_y_size = width * height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;

    jint dst_y_size = width * height;
    jbyte *dst_y = dst;
    jbyte *dst_uv = dst + dst_y_size;

    libyuv::I420ToNV12(
            (uint8_t *) src_y, width,
            (uint8_t *) src_u, width >> 1,
            (uint8_t *) src_v, width >> 1,
            (uint8_t *) dst_y, width,
            (uint8_t *) dst_uv, width,
            width, height
    );
}

void I420ToABGR(jbyte *src, int width, int height, void *dst, int dst_stride) {
    jint src_y_size = width * height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;
    libyuv::I420ToABGR(
            (uint8_t *) src_y, width,
            (uint8_t *) src_u, width >> 1,
            (uint8_t *) src_v, width >> 1,
            (uint8_t *) dst, dst_stride,
            width, height
    );
}

void I420ToNV21(jbyte *src, jbyte *dst, int width, int height) {
    jint src_y_size = width * height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;

    jint dst_y_size = width * height;
    jbyte *dst_y = dst;
    jbyte *dst_vu = dst + dst_y_size;

    libyuv::I420ToNV21(
            (uint8_t *) src_y, width,
            (uint8_t *) src_u, width >> 1,
            (uint8_t *) src_v, width >> 1,
            (uint8_t *) dst_y, width,
            (uint8_t *) dst_vu, width,
            width, height
    );
}

void I420Scale(jbyte *src, int src_width, int src_height, jbyte *dst,
                           int dst_width, int dst_height) {
    jint src_y_size = src_width * src_height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;

    jint dst_y_size = dst_width * dst_height;
    jint dst_u_size = dst_y_size >> 2;
    jbyte *dst_y = dst;
    jbyte *dst_u = dst + dst_y_size;
    jbyte *dst_v = dst + dst_y_size + dst_u_size;

    libyuv::I420Scale(
            (uint8_t *) src_y, src_width,
            (uint8_t *) src_u, src_width >> 1,
            (uint8_t *) src_v, src_width >> 1,
            src_width, src_height,
            (uint8_t *) dst_y, dst_width,
            (uint8_t *) dst_u, dst_width >> 1,
            (uint8_t *) dst_v, dst_width >> 1,
            dst_width, dst_height,
            libyuv::FilterMode::kFilterNone
    );

}

void I420Rotate(jbyte *src, jbyte *dst, int &width, int &height,
                            int degree) {
    jint src_y_size = width * height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;

    jbyte *dst_y = dst;
    jbyte *dst_u = dst + src_y_size;
    jbyte *dst_v = dst + src_y_size + src_u_size;

    libyuv::I420Rotate(
            (uint8_t *) src_y, width,
            (uint8_t *) src_u, width >> 1,
            (uint8_t *) src_v, width >> 1,
            (uint8_t *) dst_y, height,
            (uint8_t *) dst_u, height >> 1,
            (uint8_t *) dst_v, height >> 1,
            width, height, (libyuv::RotationMode) degree
    );
    // 若为 90 / 270,  则翻转宽高
    if (degree == libyuv::kRotate90 || degree == libyuv::kRotate270) {
        width += height;
        height = width - height;
        width -= height;
    }
}

void I420Mirror(jbyte *src, jbyte *dst, int width, int height) {
    jint src_y_size = width * height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;

    jbyte *dst_y = dst;
    jbyte *dst_u = dst + src_y_size;
    jbyte *dst_v = dst + src_y_size + src_u_size;

    libyuv::I420Mirror((uint8_t *) src_y, width,
                       (uint8_t *) src_u, width >> 1,
                       (uint8_t *) src_v, width >> 1,
                       (uint8_t *) dst_y, width,
                       (uint8_t *) dst_u, width >> 1,
                       (uint8_t *) dst_v, width >> 1,
                       width, height);
}

void I420Crop(jbyte *src, int src_width, int src_height, jbyte *dst, int dst_width,
                          int dst_height, int left, int top) {
    jint dst_y_size = dst_width * dst_height;
    jint dst_u_size = dst_y_size >> 2;
    jbyte *dst_y = dst;
    jbyte *dst_u = dst + dst_y_size;
    jbyte *dst_v = dst + dst_y_size + dst_u_size;

    libyuv::ConvertToI420(
            (uint8_t *) src, (size_t) src_width * src_height * 3 / 2,
            (uint8_t *) dst_y, dst_width,
            (uint8_t *) dst_u, dst_width >> 1,
            (uint8_t *) dst_v, dst_width >> 1,
            left, top,
            src_width, src_height,
            dst_width, dst_height,
            libyuv::kRotate0, libyuv::FOURCC_I420
    );

}

extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_convertNV21ToI420(JNIEnv *env, jclass type, jbyteArray src,
                                           jbyteArray dst,
                                           jint width, jint height) {
        jbyte *src1 = env->GetByteArrayElements(src, NULL);
        jbyte *dst1 = env->GetByteArrayElements(dst, NULL);
        // 执行转换 I420 -> NV12 的转换
        NV21ToI420(src1, dst1, width, height);
        // 释放资源
        env->ReleaseByteArrayElements(src, src1, 0);
        env->ReleaseByteArrayElements(dst, dst1, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_convertI420ToNV12(JNIEnv *env, jclass, jbyteArray i420_src, jbyteArray nv12_dst, int width,
                       int height) {
    jbyte *src = env->GetByteArrayElements(i420_src, NULL);
    jbyte *dst = env->GetByteArrayElements(nv12_dst, NULL);
    // 执行转换 I420 -> NV12 的转换
    I420ToNV12(src, dst, width, height);
    // 释放资源
    env->ReleaseByteArrayElements(i420_src, src, 0);
    env->ReleaseByteArrayElements(nv12_dst, dst, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_compressI420(JNIEnv *env, jclass, jbyteArray i420_src, int src_width,
                  int src_height, jbyteArray i420_dst, int dst_width,
                  int dst_height, int degree, jboolean isMirror) {
    jbyte *src = env->GetByteArrayElements(i420_src, NULL);
    const int dst_size = dst_width * dst_height * 3 / 2;
    // step1: 缩放处理
    jbyte *scaled = src;
    if (src_width != dst_width || src_height != dst_height) {
        scaled = new jbyte[dst_size];
        I420Scale(src, src_width, src_height, scaled, dst_width,
                              dst_height);
    }
    // step2: 旋转处理
    jbyte *rotated = scaled;
    if (degree != 0) {
        rotated = new jbyte[dst_size];
        // 若为 90/270 旋转之后会反转 width 和 height
        I420Rotate(scaled, rotated, dst_width, dst_height, degree);
        if (scaled != src) {
            delete[]scaled;
        }
    }
    // step3: 镜像处理
    jbyte *mirrored = rotated;
    if (isMirror) {
        mirrored = new jbyte[dst_size];
        I420Mirror(rotated, mirrored, dst_width, dst_height);
        if (rotated != src) {
            delete[]rotated;
        }
    }
    // step4: 将数据拷贝到 dst 中
    jbyte *dst = env->GetByteArrayElements(i420_dst, NULL);
    memcpy(dst, mirrored, (size_t) dst_size);
    // 释放资源
    if (mirrored != src) {
        delete[]mirrored;
    }
    env->ReleaseByteArrayElements(i420_src, src, 0);
    env->ReleaseByteArrayElements(i420_dst, dst, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_convertI420ToBitmap(JNIEnv *env, jclass, jbyteArray nv21_src, jobject bitmap, int width,
                         int height) {
//    jbyte *src = env->GetByteArrayElements(nv21_src, NULL);
//    // 锁定画布
//    void *dst_argb;
//    AndroidBitmap_lockPixels(env, bitmap, &dst_argb);
//    // 获取 bitmap 的信息
//    AndroidBitmapInfo info;
//    AndroidBitmap_getInfo(env, bitmap, &info);
//    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
//        // ABGR 的 stride 为 4 * width
//        I420ToABGR(src, width, height, dst_argb, info.stride);
//    } else {
//        // ignore.
//    }
//    // 解锁画布
//    AndroidBitmap_unlockPixels(env, bitmap);
//    // 释放通过 jbyteArray 创建的 jbyte*
//    env->ReleaseByteArrayElements(nv21_src, src, 0);
}





