package com.hyj.libyuv;

import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

/**
 * <pre>
 *     libyuv、libjpeg库的测试项目
 * </pre>
 * Author：hyj
 * Date：2020/12/7 17:23
 */
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    /**
     * mjpeg播放、解码类
     *
     * @param view
     */
    public void mjpegPlay(View view) {
        MjpegPlayActivity.startActivity(this);
    }
}