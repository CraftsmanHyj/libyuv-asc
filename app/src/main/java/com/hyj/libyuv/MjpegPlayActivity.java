package com.hyj.libyuv;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.util.Log;
import android.widget.RadioButton;

import androidx.appcompat.app.AppCompatActivity;

import com.hyj.libyuv.view.ETVideoView;
import com.hyj.newversion.YuvConvert;

/**
 * <pre>
 *     1.mjpeg视频流播放
 *     2.将mjpeg视频流解码成i420的byte[]数据
 * </pre>
 * Author：hyj
 * Date：2020/12/7 17:22
 */
public class MjpegPlayActivity extends AppCompatActivity {
    private final String TAG = MjpegPlayActivity.class.getSimpleName();

    private RadioButton rbBitmap;
    private ETVideoView etvSurfaceView;

    private String playUrl = "http://10.10.1.1:8196/?action=stream";

    public static void startActivity(Context cxt) {
        Intent intent = new Intent(cxt, MjpegPlayActivity.class);
        cxt.startActivity(intent);
    }

    @Override
    protected void onDestroy() {
        if (null != etvSurfaceView) {
            etvSurfaceView.stopWorker();
        }
        super.onDestroy();
    }

    @Override
    public void onResume() {
        super.onResume();
        etvSurfaceView.setPlayStatus(true);
    }

    @Override
    public void onPause() {
        super.onPause();
        etvSurfaceView.setPlayStatus(false);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mjpeg_play_activity);

        initUI();
        initListener();
    }

    private void initUI() {
        rbBitmap = findViewById(R.id.rbBitmap2i420);
        etvSurfaceView = findViewById(R.id.etvSurfaceView);
        etvSurfaceView.setMjpegPlayUrl(playUrl);
    }

    private void initListener() {
        etvSurfaceView.setOnBitmapCallback(new ETVideoView.OnBitmapCallback() {
            @Override
            public void onBitmap(Bitmap bitmap, byte[] bitmapByte, int width, int height) {
                try {
                    if (rbBitmap.isChecked()) {
                        onI420Data(bitmap, bitmapByte, width, height);
                    } else {
                        onI420DataWithMjpeg(bitmap, bitmapByte, width, height);
                    }
                } catch (Throwable throwable) {
                    throwable.printStackTrace();
                }
            }
        });
    }

    /**
     * 数据解码发送至腾讯
     */
    private void onI420Data(Bitmap bitmap, byte[] bitmapByte, int width, int height) throws Throwable {
        Log.v(TAG, "bitmap直接用C解码，转成i420 byte[]数据");

        //将bitmap数据解码为i420的byte[]数据，然后传给腾讯
        byte[] frameByte = YuvConvert.bitmap2i420WithC(bitmap);
        if (null == frameByte) {
            return;
        }

        //将数据传给腾讯SDK
//        baseCallPresenter.sendCustomVideoData(frameByte, width, height);
    }

    /**
     * 数据解码发送至腾讯
     */
    private void onI420DataWithMjpeg(Bitmap bitmap, byte[] bitmapByte, int width, int height) throws Throwable {
        Log.v(TAG, "使用MJPEG的byte[]转成i420的byte[]数据");

        //mjpeg转i420方法，必须显示设置当前jpg图片宽高，不能用bitmap中的,因为有缩放
        width = 1280;
        height = 720;
        byte[] frameByte = YuvConvert.mjpeg2i420(bitmapByte, width, height);
        if (null == frameByte) {
            return;
        }

        //将数据传送给腾讯SDK
//        baseCallPresenter.sendCustomVideoData(frameByte, width, height);
    }
}