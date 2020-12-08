package cn.ibingli.library.yuv;

import android.graphics.Bitmap;

/**
 * Created by Administrator on 2017/6/4.
 */
public class YuvUtils {
    static {
        System.loadLibrary("yuv");
    }

    //argb 转yuv i420
    public static native void argbtoi420(byte[] src_argb, int src_stride_argb,
                                         byte[] dst_y, int dst_stride_y,
                                         byte[] dst_u, int dst_stride_u,
                                         byte[] dst_v, int dst_stride_v,
                                         int width, int height);

    //对yuv 片数据 进行处理 包括裁剪和旋转
    public static native void convertToI420(byte[] src_frame, int src_size,
                                            byte[] dst_y, int dst_stride_y,
                                            byte[] dst_u, int dst_stride_u,
                                            byte[] dst_v, int dst_stride_v,
                                            int crop_x, int crop_y,
                                            int src_width, int src_height,
                                            int crop_width, int crop_height,
                                            int rotation,
                                            int format);

    //将yuv数据转为argb
    public static native void convertToArgb(byte[] src_frame, int src_size,
                                            byte[] dst_argb, int dst_stride_argb,
                                            int crop_x, int crop_y,
                                            int src_width, int src_height,
                                            int crop_width, int crop_height,
                                            int rotation,
                                            int format);

    /**
     * 将I420转化为NV21
     *
     * @param i420Src 原始I420数据
     * @param nv21Src 转化后的NV21数据
     * @param width   输出的宽
     * @param width   输出的高
     **/
    public static native void yuvI420ToNV21(byte[] i420Src, byte[] nv21Src, int width, int height);

    /**
     * 将bitmap的数据转换成i420的byte[]数据
     *
     * @param dst    存放转换后的i420数据
     * @param bmp    需要解析的图片
     * @param width  图片宽
     * @param height 图片高
     */
    public static native void bitmap2i420WithC(byte[] dst, Bitmap bmp, int width, int height);

    /**
     * 直接将mjpeg相机里面出来的jpg图片的byte[]转换成i420的byte[]数据
     *
     * @param src      jpg的byte[]数据源
     * @param src_size jpg byte[]数据长度
     * @param dst      存放i420的byte[]结构
     * @param width    jpg的图片宽
     * @param height   jpg的图片高
     */
    public static native void convertMJPEGToI420(byte[] src, int src_size, byte[] dst, int width, int height);
}