package com.hyj.libyuv.view;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MySurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    private static final float MAX_ZOOM_SCALE = 8.0f;
    private static final float MIN_ZOOM_SCALE = 1.0f;
    private static final float FLOAT_TYPE = 1.0f;
    private float mCurrentMaxScale = MAX_ZOOM_SCALE;
    private float mCurrentScale = 1.0f;

    protected Rect mRectSrc = new Rect(); // used for render image.
    protected Rect mRectDes = new Rect(); // used for store size of monitor.

    public boolean isInit;//是否已经初始化
    private int mCenterX, mCenterY;
    int mSurfaceHeight, mSurfaceWidth, mImageHeight, mImageWidth;

    private SurfaceHolder mSurHolder = null;
    private Bitmap mBitmap;

    public MySurfaceView(Context context) {
        this(context, null);
    }

    public MySurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mSurHolder = getHolder();
        mSurHolder.addCallback(this);
    }

    private void init() {
        mCurrentMaxScale = Math.max(MIN_ZOOM_SCALE,
                4 * Math.min(FLOAT_TYPE * mImageHeight / mSurfaceHeight, 1.0f * mImageWidth / mSurfaceWidth));

        mCurrentScale = MIN_ZOOM_SCALE;
        mCenterX = mImageWidth / 2;
        mCenterY = mImageHeight / 2;

        calcRect();
    }

    private void calcRect() {
        int w, h;
        float imageRatio, surfaceRatio;
        imageRatio = FLOAT_TYPE * mImageWidth / mImageHeight;
        surfaceRatio = FLOAT_TYPE * mSurfaceWidth / mSurfaceHeight;

        if (imageRatio < surfaceRatio) {
            h = mSurfaceHeight;
            w = (int) (h * imageRatio);
        } else {
            w = mSurfaceWidth;
            h = (int) (w / imageRatio);
        }

        if (mCurrentScale > MIN_ZOOM_SCALE) {
            w = Math.min(mSurfaceWidth, (int) (w * mCurrentScale));
            h = Math.min(mSurfaceHeight, (int) (h * mCurrentScale));
        } else {
            mCurrentScale = MIN_ZOOM_SCALE;
        }

        mRectDes.left = (mSurfaceWidth - w) / 2;
        mRectDes.top = (mSurfaceHeight - h) / 2;
        mRectDes.right = mRectDes.left + w;
        mRectDes.bottom = mRectDes.top + h;

        int h2, w2;
        float curImageRatio = FLOAT_TYPE * w / h;
        if (curImageRatio > imageRatio) {
            h2 = (int) (mImageHeight / mCurrentScale);
            w2 = (int) (h2 * curImageRatio);
        } else {
            w2 = (int) (mImageWidth / mCurrentScale);
            h2 = (int) (w2 / curImageRatio);
        }

        mRectSrc.left = mCenterX - w2 / 2;
        mRectSrc.top = mCenterY - h2 / 2;
        mRectSrc.right = mRectSrc.left + w2;
        mRectSrc.bottom = mRectSrc.top + h2;
    }

    public void setBitmap(Bitmap bitmap) {
        try {
            if (bitmap == null || bitmap.isRecycled()) {
                return;
            }

            synchronized (MySurfaceView.class) {
                if (mBitmap != null && !mBitmap.isRecycled()) {
                    mBitmap.recycle();
                }

                mBitmap = bitmap;
                if (mImageHeight != mBitmap.getHeight() || mImageWidth != mBitmap.getWidth()) {
                    mImageHeight = mBitmap.getHeight();
                    mImageWidth = mBitmap.getWidth();
                    init();
                }

                isInit = true;
                showBitmap();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void showBitmap() {
        if (!isInit) {
            return;
        }

        try {
            if (isValid()) {
                Canvas c = getHolder().lockCanvas();
                if (c != null && mBitmap != null) {
                    c.drawColor(Color.BLACK);
                    c.drawBitmap(mBitmap, mRectSrc, mRectDes, null);
                }
                getHolder().unlockCanvasAndPost(c);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private boolean isValid() {
        return (mSurHolder != null && mSurHolder.getSurface().isValid());
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
    }

    // 初始化
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        synchronized (MySurfaceView.class) {
            mRectDes.set(0, 0, width, height);
            mSurfaceHeight = height;
            mSurfaceWidth = width;
            init();

            if (mBitmap != null) {
                showBitmap();
            }
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }

    /**
     * 图片偏移后把图像重新居中, 这里没有用到
     */
    private void adjustCenter() {
        int w = mRectSrc.right - mRectSrc.left;
        int h = mRectSrc.bottom - mRectSrc.top;

        if (mCenterX - w / 2 < 0) {
            mCenterX = w / 2;
            mRectSrc.left = 0;
            mRectSrc.right = w;
        } else if (mCenterX + w / 2 >= mImageWidth) {
            mCenterX = mImageWidth - w / 2;
            mRectSrc.right = mImageWidth;
            mRectSrc.left = mRectSrc.right - w;
        } else {
            mRectSrc.left = mCenterX - w / 2;
            mRectSrc.right = mRectSrc.left + w;
        }

        if (mCenterY - h / 2 < 0) {
            mCenterY = h / 2;
            mRectSrc.top = 0;
            mRectSrc.bottom = h;
        } else if (mCenterY + h / 2 >= mImageHeight) {
            mCenterY = mImageHeight - h / 2;
            mRectSrc.bottom = mImageHeight;
            mRectSrc.top = mRectSrc.bottom - h;
        } else {
            mRectSrc.top = mCenterY - h / 2;
            mRectSrc.bottom = mRectSrc.top + h;
        }
    }
}