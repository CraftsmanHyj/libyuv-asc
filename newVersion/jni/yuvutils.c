//
// Created by Administrator on 2017/6/4.
// ndk中打印日志方法：http://www.linyibin.cn/2016/01/04/JNI-LogCat/
#include <android/bitmap.h>
#include "include/libyuv.h"
#include <jni.h>

void Java_com_example_libyuv_Test_argbtoi420(JNIEnv *env, jobject thiz,
                                             jbyteArray src_argb, int src_stride_argb,
                                             jbyteArray dst_y, int dst_stride_y,
                                             jbyteArray dst_u, int dst_stride_u,
                                             jbyteArray dst_v, int dst_stride_v,
                                             int width, int height) {
    uint8_t *rgbBuffer = (uint8_t *) (*env)->GetByteArrayElements(env, src_argb, 0);
    uint8_t *yBuffer = (uint8_t *) (*env)->GetByteArrayElements(env, dst_y, 0);
    uint8_t *uBuffer = (uint8_t *) (*env)->GetByteArrayElements(env, dst_u, 0);
    uint8_t *vBuffer = (uint8_t *) (*env)->GetByteArrayElements(env, dst_v, 0);

    ABGRToI420(rgbBuffer, src_stride_argb, yBuffer, dst_stride_y, uBuffer, dst_stride_u, vBuffer,
               dst_stride_v, width, height);

    (*env)->ReleaseByteArrayElements(env, src_argb, rgbBuffer, 0);
    (*env)->ReleaseByteArrayElements(env, dst_y, yBuffer, 0);
    (*env)->ReleaseByteArrayElements(env, dst_u, uBuffer, 0);
    (*env)->ReleaseByteArrayElements(env, dst_v, vBuffer, 0);
}

void Java_com_example_libyuv_Test_convertToI420(JNIEnv *env, jobject thiz,
                                                jbyteArray src_frame, int src_size,
                                                jbyteArray dst_y, int dst_stride_y,
                                                jbyteArray dst_u, int dst_stride_u,
                                                jbyteArray dst_v, int dst_stride_v,
                                                int crop_x, int crop_y,
                                                int src_width, int src_height,
                                                int crop_width, int crop_height,
                                                int rotation,
                                                int format) {
    uint8_t *yuvFrame = (uint8_t *) (*env)->GetByteArrayElements(env, src_frame, 0);
    uint8_t *yBuffer = (uint8_t *) (*env)->GetByteArrayElements(env, dst_y, 0);
    uint8_t *uBuffer = (uint8_t *) (*env)->GetByteArrayElements(env, dst_u, 0);
    uint8_t *vBuffer = (uint8_t *) (*env)->GetByteArrayElements(env, dst_v, 0);

    ConvertToI420(yuvFrame, src_size, yBuffer, dst_stride_y, uBuffer, dst_stride_u, vBuffer,
                  dst_stride_v, crop_x, crop_y, src_width, src_height, crop_width, crop_height,
                  kRotate0, FOURCC_IYUV);

    (*env)->ReleaseByteArrayElements(env, src_frame, yuvFrame, 0);
    (*env)->ReleaseByteArrayElements(env, dst_y, yBuffer, 0);
    (*env)->ReleaseByteArrayElements(env, dst_u, uBuffer, 0);
    (*env)->ReleaseByteArrayElements(env, dst_v, vBuffer, 0);
}

void Java_com_example_libyuv_Test_convertToArgb(JNIEnv *env, jobject thiz,
                                                jbyteArray src_frame, int src_size,
                                                jbyteArray dst_argb, int dst_stride_argb,
                                                int crop_x, int crop_y,
                                                int src_width, int src_height,
                                                int crop_width, int crop_height,
                                                int rotation,
                                                int format) {
    uint8_t *yuvFrame = (uint8_t *) (*env)->GetByteArrayElements(env, src_frame, 0);
    uint8_t *rgbBuffer = (uint8_t *) (*env)->GetByteArrayElements(env, dst_argb, 0);

    ConvertToARGB(yuvFrame, src_size, rgbBuffer, dst_stride_argb, crop_x, crop_y, src_width,
                  src_height, crop_width, crop_height, kRotate0, FOURCC_IYUV);
    (*env)->ReleaseByteArrayElements(env, src_frame, yuvFrame, 0);
    (*env)->ReleaseByteArrayElements(env, dst_argb, rgbBuffer, 0);
}

JNIEXPORT void JNICALL
Java_com_hyj_newversion_yuv_YuvUtils_argbtoi420(JNIEnv *env, jclass type, jbyteArray src_argb_,
                                                jint src_stride_argb, jbyteArray dst_y_,
                                                jint dst_stride_y,
                                                jbyteArray dst_u_, jint dst_stride_u,
                                                jbyteArray dst_v_,
                                                jint dst_stride_v, jint width, jint height) {
    uint8_t *src_argb = (uint8_t *) (*env)->GetByteArrayElements(env, src_argb_, 0);
    uint8_t *dst_y = (uint8_t *) (*env)->GetByteArrayElements(env, dst_y_, 0);
    uint8_t *dst_u = (uint8_t *) (*env)->GetByteArrayElements(env, dst_u_, 0);
    uint8_t *dst_v = (uint8_t *) (*env)->GetByteArrayElements(env, dst_v_, 0);

    ABGRToI420(src_argb, src_stride_argb, dst_y, dst_stride_y, dst_u, dst_stride_u, dst_v,
               dst_stride_v, width, height);

    (*env)->ReleaseByteArrayElements(env, src_argb_, src_argb, 0);
    (*env)->ReleaseByteArrayElements(env, dst_y_, dst_y, 0);
    (*env)->ReleaseByteArrayElements(env, dst_u_, dst_u, 0);
    (*env)->ReleaseByteArrayElements(env, dst_v_, dst_v, 0);
}

JNIEXPORT void JNICALL
Java_com_hyj_newversion_yuv_YuvUtils_convertToI420(JNIEnv *env, jclass type, jbyteArray src_frame_,
                                                   jint src_size, jbyteArray dst_y_,
                                                   jint dst_stride_y,
                                                   jbyteArray dst_u_, jint dst_stride_u,
                                                   jbyteArray dst_v_,
                                                   jint dst_stride_v, jint crop_x, jint crop_y,
                                                   jint src_width, jint src_height, jint crop_width,
                                                   jint crop_height, jint rotation, jint format) {
    uint8_t *src_frame = (uint8_t *) (*env)->GetByteArrayElements(env, src_frame_, 0);
    uint8_t *dst_y = (uint8_t *) (*env)->GetByteArrayElements(env, dst_y_, 0);
    uint8_t *dst_u = (uint8_t *) (*env)->GetByteArrayElements(env, dst_u_, 0);
    uint8_t *dst_v = (uint8_t *) (*env)->GetByteArrayElements(env, dst_v_, 0);

    ConvertToI420(src_frame, src_size, dst_y, dst_stride_y, dst_u, dst_stride_u, dst_v,
                  dst_stride_v, crop_x, crop_y, src_width, src_height, crop_width, crop_height,
                  kRotate0, FOURCC_IYUV);

    (*env)->ReleaseByteArrayElements(env, src_frame_, src_frame, 0);
    (*env)->ReleaseByteArrayElements(env, dst_y_, dst_y, 0);
    (*env)->ReleaseByteArrayElements(env, dst_u_, dst_u, 0);
    (*env)->ReleaseByteArrayElements(env, dst_v_, dst_v, 0);
}

JNIEXPORT void JNICALL
Java_com_hyj_newversion_yuv_YuvUtils_convertToArgb(JNIEnv *env, jclass type, jbyteArray src_frame_,
                                                   jint src_size, jbyteArray dst_argb_,
                                                   jint dst_stride_argb, jint crop_x, jint crop_y,
                                                   jint src_width, jint src_height, jint crop_width,
                                                   jint crop_height, jint rotation, jint format) {
    uint8_t *src_frame = (uint8_t *) (*env)->GetByteArrayElements(env, src_frame_, 0);
    uint8_t *dst_argb = (uint8_t *) (*env)->GetByteArrayElements(env, dst_argb_, 0);

    ConvertToARGB(src_frame, src_size, dst_argb, dst_stride_argb, crop_x, crop_y, src_width,
                  src_height, crop_width, crop_height, kRotate0, FOURCC_IYUV);

    (*env)->ReleaseByteArrayElements(env, src_frame_, src_frame, 0);
    (*env)->ReleaseByteArrayElements(env, dst_argb_, dst_argb, 0);
}

JNIEXPORT void JNICALL
Java_com_hyj_newversion_yuv_YuvUtils_yuvI420ToNV21(JNIEnv *env, jclass type, jbyteArray i420Src_,
                                                   jbyteArray nv21Src_, jint width, jint height) {
    jbyte *i420Src = (*env)->GetByteArrayElements(env, i420Src_, 0);
    jbyte *nv21Src = (*env)->GetByteArrayElements(env, nv21Src_, 0);

    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = i420Src;
    jbyte *src_i420_u_data = i420Src + src_y_size;
    jbyte *src_i420_v_data = i420Src + src_y_size + src_u_size;

    jbyte *src_nv21_y_data = nv21Src;
    jbyte *src_nv21_vu_data = nv21Src + src_y_size;
    I420ToNV21(
            (const uint8_t *) src_i420_y_data, width,
            (const uint8_t *) src_i420_u_data, width >> 1,
            (const uint8_t *) src_i420_v_data, width >> 1,
            (uint8_t *) src_nv21_y_data, width,
            (uint8_t *) src_nv21_vu_data, width,
            width, height);

    (*env)->ReleaseByteArrayElements(env, i420Src_, i420Src, 0);
    (*env)->ReleaseByteArrayElements(env, nv21Src_, nv21Src, 0);
}

JNIEXPORT void JNICALL
Java_com_hyj_newversion_yuv_YuvUtils_bitmap2i420WithC(JNIEnv *env, jclass type,
                                                 jbyteArray dst_argb_,
                                                 jobject bitmap,
                                                 jint width, jint height) {
    uint8_t *dst = (uint8_t *) (*env)->GetByteArrayElements(env, dst_argb_, 0);

    // 获取 bitmap 的信息
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);

    int src_width=info.width;
    int src_height=info.height;

    int src_stride_argb=src_width*4;//info.stride
    int dst_plane1_size = src_width * src_height;

    uint8_t *dst_buffer_y = dst;
    uint8_t *dst_buffer_u = dst_buffer_y + dst_plane1_size;
    uint8_t *dst_buffer_v = dst_buffer_u + dst_plane1_size / 4;

    int dst_stride_y=width;
    int dst_stride_u= (width + 1) / 2;
    int dst_stride_v=(width + 1) / 2;

    void *dst_argb;
    AndroidBitmap_lockPixels(env, bitmap, &dst_argb);//锁定画布

   ABGRToI420(dst_argb, src_stride_argb,
           dst_buffer_y, dst_stride_y,
           dst_buffer_u, dst_stride_u,
           dst_buffer_v,dst_stride_v,
           width, height);

    AndroidBitmap_unlockPixels(env, bitmap);// 解锁画布
    (*env)->ReleaseByteArrayElements(env, dst_argb_, dst, 0);
}

JNIEXPORT void JNICALL
Java_com_hyj_newversion_yuv_YuvUtils_convertMJPEGToI420(JNIEnv *env, jclass type, jbyteArray src_frame_,
                                                   jint src_size, jbyteArray dst_,
                                                   jint src_width, jint src_height) {
    int dst_plane1_stride = src_width;
    int dst_plane1_size = src_width * src_height;
    uint8_t *src_frame = (uint8_t *) (*env)->GetByteArrayElements(env, src_frame_, 0);
    uint8_t *dst = (uint8_t *) (*env)->GetByteArrayElements(env, dst_, 0);
    uint8_t *dst_buffer_y = dst;
    uint8_t *dst_buffer_u = dst_buffer_y + dst_plane1_size;
    uint8_t *dst_buffer_v = dst_buffer_u + dst_plane1_size / 4;

//    MJPGToI420(src_frame,src_size,
//               dst_buffer_y,dst_plane1_stride,
//               dst_buffer_u,(int)dst_plane1_stride >> 1,
//               dst_buffer_v,(int)dst_plane1_stride >> 1,
//               src_width,src_height,
//               src_width,src_height);

    (*env)->ReleaseByteArrayElements(env, src_frame_, src_frame, 0);
    (*env)->ReleaseByteArrayElements(env, dst_, dst, 0);
}