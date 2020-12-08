package com.hyj.libyuv.view;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Paint;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * 从硬件中读入数据及分析处理数据
 *
 * @author Cola
 */
public class ETVideoView extends MySurfaceView implements View.OnTouchListener {
    private SurfaceHolder mHold;
    private ImageThread mImageThread;

    private Paint mPaint;
    private BitmapFactory.Options mOptions;

    public byte[] mCacheByteArray;
    private List<Thread> mThreadList = new ArrayList<>();
    private boolean mNetWorkStatu = false;//当前播放状态
    private String mjpegPlayUrl;//motion jpeg视频播放的地址

    private OnDoubleTabListener onDoubleTabListener;
    private OnBitmapCallback onBitmapCallback;

    /**
     * @param onBitmapCallback
     */
    public void setOnBitmapCallback(OnBitmapCallback onBitmapCallback) {
        this.onBitmapCallback = onBitmapCallback;
    }

    /**
     * 设置双击事件
     *
     * @param onDoubleTabListener
     */
    public ETVideoView setOnDoubleTabListener(OnDoubleTabListener onDoubleTabListener) {
        this.onDoubleTabListener = onDoubleTabListener;
        return this;
    }

    /**
     * 设置Mjpeg视频流播放地址
     *
     * @param mjpegPlayUrl motion jpeg 视频播放地址
     */
    public ETVideoView setMjpegPlayUrl(String mjpegPlayUrl) {
        this.mjpegPlayUrl = mjpegPlayUrl;
        return this;
    }

    /**
     * 设置当前播放状态
     *
     * @param playStatus true：播放;false：停止;
     */
    public ETVideoView setPlayStatus(boolean playStatus) {
        this.mNetWorkStatu = playStatus;

        if (!mNetWorkStatu && mImageThread != null) {
            mImageThread.interrupt();
        } else if (mImageThread == null || !mImageThread.isAlive()) {
            connect();
        }
        return this;
    }

    public ETVideoView(Context paramContext) {
        this(paramContext, null);
    }

    public ETVideoView(Context paramContext, AttributeSet paramAttributeSet) {
        super(paramContext, paramAttributeSet);
        Init();
        initListener();
    }

    private void Init() {
        setFocusable(false);
        setFocusableInTouchMode(false);
        setKeepScreenOn(true);
        this.mPaint = new Paint();
        this.mPaint.setAntiAlias(true);
        this.mPaint.setStyle(Paint.Style.FILL);

        this.mOptions = new BitmapFactory.Options();
        this.mOptions.inSampleSize = 2;

        //原本播放器中使用的是Bitmap.Config.RGB_565,但是使用libyuv转i420的时候需要ARGB_8888
//        this.mOptions.inPreferredConfig = Bitmap.Config.RGB_565;
        this.mOptions.inPreferredConfig = Bitmap.Config.ARGB_8888;

        this.mOptions.inPurgeable = true;
        this.mOptions.inInputShareable = true;
        this.mOptions.inDither = false;
        this.mOptions.inJustDecodeBounds = false;
        this.mHold = getHolder();
        this.mHold.addCallback(this);
    }

    private void initListener() {
        setOnTouchListener(this);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        mGesture.onTouchEvent(event);
        return true;
    }

    private GestureDetector mGesture = new GestureDetector(getContext(), new GestureDetector.SimpleOnGestureListener() {
        @Override
        public boolean onDoubleTap(MotionEvent e) {
            if (null != onDoubleTabListener) {
                onDoubleTabListener.onDoubleAction();
            }
            return true;
        }
    });

    /**
     * 用户双击事件
     */
    public interface OnDoubleTabListener {
        void onDoubleAction();
    }

    /**
     * bitmap图片回调
     */
    public interface OnBitmapCallback {
        void onBitmap(Bitmap bitmap, byte[] bitmapByte, int width, int height);
    }

    /**
     * 连接MJPEG服务
     */
    public void connect() {
        try {
            if (TextUtils.isEmpty(mjpegPlayUrl)) {
                return;
            }

            stopWorker();
            this.mImageThread = new ImageThread(mjpegPlayUrl);
            this.mImageThread.start();
            synchronized (mThreadList) {
                mThreadList.add(mImageThread);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void surfaceCreated(SurfaceHolder holder) {
        super.surfaceCreated(holder);
        mNetWorkStatu = true;

        connect();
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        super.surfaceDestroyed(holder);

        stopWorker();
    }

    public void stopWorker() {
        synchronized (mThreadList) {
            if (mThreadList.size() > 0) {
                for (Thread t : mThreadList) {
                    if (t == null) {
                        continue;
                    }

                    ((ImageThread) t).cancel();
                    t.interrupt();
                    t = null;//必须释放资源，否则会一直在后台读取http中的视频流
                }

                mThreadList.clear();
            }
        }
    }

    private class ImageThread extends Thread {
        //单次接收到的数据
        private byte[] buffer = new byte[1500];
        //缓存的图片数据
        private byte[] cache = new byte[4500000];
        private boolean isFindHead = false;
        private boolean isRun = false;
        private InputStream mIn = null;
        private int currentDataLength;
        //图片开始的索引
        private int headIndex;
        //图片结束标识索引
        private int tailIndex;
        private String playUrl;
        private HttpURLConnection conn;

        public void cleanCache() {
            if (cache != null) {
                Arrays.fill(cache, (byte) 0);
            }

            if (buffer != null) {
                Arrays.fill(buffer, (byte) 0);
            }

            buffer = null;
            cache = null;
        }

        public ImageThread(String mjpegUrl) {
            Arrays.fill(this.buffer, 0, this.buffer.length, (byte) 0);
            Arrays.fill(this.cache, 0, this.cache.length, (byte) 0);

            this.isRun = true;
            this.playUrl = mjpegUrl;
        }

        private void copy(byte[] dest, int destStart, byte[] src, int start, int size) {
            System.arraycopy(src, start, dest, destStart, size);
        }

        private byte[] copyOfRange(byte[] scrArrayOfByte, int start, int end) {
            int size = end - start;
            byte[] decArrayOfByte = new byte[size];
            try {
                System.arraycopy(scrArrayOfByte, start, decArrayOfByte, 0, size);
                return decArrayOfByte;
            } catch (Exception localException) {
                localException.printStackTrace();
            }
            return decArrayOfByte;
        }

        private int findHead(byte[] paramArrayOfByte) {
            int i = paramArrayOfByte.length;
            for (int j = 0; j < i; j++) {
                if (j >= i - 1) {
                    return -1;
                }

                if ((paramArrayOfByte[j] == -1) && (paramArrayOfByte[(j + 1)] == -40)) {
                    return j;
                }
            }

            return -1;
        }

        private int findTail(byte[] paramArrayOfByte) {
            int i = paramArrayOfByte.length;
            for (int j = 0; j < i; j++) {
                if (j >= i - 1) {
                    return -1;
                }

                if ((paramArrayOfByte[j] == -1) && (paramArrayOfByte[(j + 1)] == -39)) {
                    return j + 1;
                }
            }

            return -1;
        }

        public void cancel() {
            isRun = false;
            try {
                if (mIn != null) {
                    mIn.close();
                }
                if (conn != null) {
                    conn.disconnect();
                }
            } catch (Exception e1) {
                e1.printStackTrace();
            }
            cleanCache();
        }

        public void run() {
            if (TextUtils.isEmpty(playUrl)) {
                return;
            }

            while (true) {
                if (!isRun) {
                    break;
                }

                try {
                    URL httpUrl = new URL(playUrl);
                    conn = (HttpURLConnection) httpUrl.openConnection();
                    conn.setConnectTimeout(5000);// 请求超时
                    conn.setReadTimeout(5000);// 读取超时
                    conn.setRequestMethod("GET");

                    if (200 != conn.getResponseCode()) {
                        return;
                    }

                    Thread.sleep(500L);
                    this.mIn = conn.getInputStream();
                } catch (IOException e) {
                    isRun = false;
                    if (mNetWorkStatu) {
                        ETVideoView.this.connect();
                    }

                    interrupt();
                    return;
                } catch (InterruptedException e) {
                    try {
                        if (mIn != null) {
                            mIn.close();
                        }
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }

                    cleanCache();
                    break;
                }

                while (true) {  //开始读取图片
                    try {
                        Arrays.fill(this.buffer, 0, this.buffer.length, (byte) 0);
                        int dataLen = this.mIn.read(this.buffer);//先读出1500字节
                        if (dataLen <= 0) { //如果满了返回-1
                            continue;
                        }

                        if (this.isFindHead) {
                            copyData(dataLen);
                            continue;
                        }

                        this.currentDataLength = 0;
                        this.headIndex = findHead(this.buffer);
                        if (this.headIndex < 0) {
                            continue;
                        }

                        copy(this.cache, 0, this.buffer, this.headIndex, dataLen - this.headIndex);
                        this.currentDataLength += dataLen - this.headIndex;
                        this.isFindHead = true;
                    } catch (Exception localException1) {
                        try {
                            if (mIn != null) {
                                mIn.close();
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }

                        isRun = false;
                        if (mNetWorkStatu) {
                            ETVideoView.this.connect();
                        }

                        interrupt();
                        cleanCache();
                        return;
                    }
                }
            }
        }

        private boolean copyData(int i) {
            this.tailIndex = findTail(this.buffer);
            if (this.tailIndex > 0) {   //找到尾巴
                copy(this.cache, this.currentDataLength, this.buffer, 0, 1 + this.tailIndex);
                this.currentDataLength += 1 + this.tailIndex;
                try {
                    Bitmap localBitmap = BitmapFactory.decodeByteArray(this.cache, 0, this.currentDataLength, ETVideoView.this.mOptions);

                    if (localBitmap != null) {
                        ETVideoView.this.setBitmap(localBitmap);
                        if (headIndex == 0) {
                            mCacheByteArray = new byte[this.currentDataLength];
                            System.arraycopy(cache, 0, mCacheByteArray, 0, currentDataLength);
                        }
                        send2Callback(localBitmap, mCacheByteArray);
                    }

                    //将缓存数据置0
                    Arrays.fill(this.cache, 0, this.currentDataLength, (byte) 0);
                    this.currentDataLength = 0;
                    //如果此次接收的数据还包括下一张图片的数据, 则复制到缓存里面
                    if (this.tailIndex != i - 1) {
                        byte[] arrayOfByte1 = copyOfRange(this.buffer, 1 + this.tailIndex, i);
                        if (arrayOfByte1.length > 0) {
                            this.headIndex = findHead(arrayOfByte1);
                            if (this.headIndex >= 0) {
                                copy(this.cache, 0, arrayOfByte1, this.headIndex, arrayOfByte1.length - this.headIndex);
                                this.currentDataLength += arrayOfByte1.length - this.headIndex;
                                this.isFindHead = true;
                            }
                        }
                    }
                } catch (Exception localException3) {
                    try {
                        if (mIn != null) {
                            mIn.close();
                        }
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                    isRun = false;
                    if (mNetWorkStatu) {
                        ETVideoView.this.connect();
                    }
                    interrupt();
                } finally {
                }

                this.isFindHead = false;
                return true;
            } else if (i + this.currentDataLength > this.cache.length) {
                this.isFindHead = false;
            } else {// 没有超出缓存的长度，并且还没找到一张图片的尾数据，继续拼接
                copy(this.cache, this.currentDataLength, this.buffer, 0, i);
                this.currentDataLength = (i + this.currentDataLength);
            }

            return false;
        }

        /**
         * 将数据回调发送给上层视图
         *
         * @param localBitmap
         */
        private void send2Callback(final Bitmap localBitmap, final byte[] bitmapByte) {
            if (null == onBitmapCallback || null == localBitmap) {
                return;
            }

            //这里需要优化
            onBitmapCallback.onBitmap(localBitmap, bitmapByte, localBitmap.getWidth(), localBitmap.getHeight());

//            new Thread(new Runnable() {
//                @Override
//                public void run() {
//                    onBitmapCallback.onBitmap(localBitmap, bitmapByte, localBitmap.getWidth(), localBitmap.getHeight());
//                }
//            }).start();
        }
    }
}