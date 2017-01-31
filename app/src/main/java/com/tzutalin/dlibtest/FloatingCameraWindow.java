/*
 *  Copyright (C) 2016 Tzuta Lin
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  imitations under the License.
 */

package com.tzutalin.dlibtest;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Point;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.support.annotation.UiThread;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TextView;

import java.lang.ref.WeakReference;

/**
 * Created by Tzutalin on 2016/5/25
 */
public class FloatingCameraWindow {
    private static final String TAG = "FloatingCameraWindow";
    private Context mContext;
    private WindowManager.LayoutParams mWindowParam;
    private WindowManager mWindowManager;
    private FloatCamView mRootView;
    private Handler mUIHandler;

    private int mWindowWidth;
    private int mWindowHeight;

    private int mScreenMaxWidth;
    private int mScreenMaxHeight;

    private float mScaleWidthRatio = 1.0f;
    private float mScaleHeightRatio = 1.0f;

    private static final boolean DEBUG = true;

    public FloatingCameraWindow(Context context) {
        mContext = context;
        mUIHandler = new Handler(Looper.getMainLooper());

        // Get screen max size
        Point size = new Point();
        Display display = ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
            display.getSize(size);
            mScreenMaxWidth = size.x;
            mScreenMaxHeight = size.y;
        } else {
            mScreenMaxWidth = display.getWidth();
            mScreenMaxHeight = display.getHeight();
        }
        // Default window size
        mWindowWidth = mScreenMaxWidth / 2;
        mWindowHeight = mScreenMaxHeight / 2;

        mWindowWidth = mWindowWidth > 0 && mWindowWidth < mScreenMaxWidth ? mWindowWidth : mScreenMaxWidth;
        mWindowHeight = mWindowHeight > 0 && mWindowHeight < mScreenMaxHeight ? mWindowHeight : mScreenMaxHeight;
    }

    public FloatingCameraWindow(Context context, int windowWidth, int windowHeight) {
        this(context);

        if (windowWidth < 0 || windowWidth > mScreenMaxWidth || windowHeight < 0 || windowHeight > mScreenMaxHeight) {
            throw new IllegalArgumentException("Window size is illegal");
        }

        mScaleWidthRatio = (float) windowWidth / mWindowHeight;
        mScaleHeightRatio = (float) windowHeight / mWindowHeight;

        if (DEBUG) {
            Log.d(TAG, "mScaleWidthRatio: " + mScaleWidthRatio);
            Log.d(TAG, "mScaleHeightRatio: " + mScaleHeightRatio);
        }

        mWindowWidth = windowWidth;
        mWindowHeight = windowHeight;
    }

    private void init() {
        mUIHandler.postAtFrontOfQueue(new Runnable() {
            @Override
            public void run() {
                if (mWindowManager == null || mRootView == null) {
                    mWindowManager = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
                    mRootView = new FloatCamView(FloatingCameraWindow.this);
                    mWindowManager.addView(mRootView, initWindowParameter());
                }
            }
        });
    }

    public void release() {
        mUIHandler.postAtFrontOfQueue(new Runnable() {
            @Override
            public void run() {
                if (mWindowManager != null) {
                    mWindowManager.removeViewImmediate(mRootView);
                    mRootView = null;
                }
                mUIHandler.removeCallbacksAndMessages(null);
            }
        });
    }

    private WindowManager.LayoutParams initWindowParameter() {
        mWindowParam = new WindowManager.LayoutParams();

        mWindowParam.type = WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;
        mWindowParam.format = 1;
        mWindowParam.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        mWindowParam.flags = mWindowParam.flags | WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH;
        mWindowParam.flags = mWindowParam.flags | WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS;

        mWindowParam.alpha = 1.0f;

        mWindowParam.gravity = Gravity.BOTTOM | Gravity.RIGHT;
        mWindowParam.x = 0;
        mWindowParam.y = 0;
        mWindowParam.width = mWindowWidth;
        mWindowParam.height = mWindowHeight;
        return mWindowParam;
    }

    public void setRGBBitmap(final Bitmap rgb) {
        checkInit();
        mUIHandler.post(new Runnable() {
            @Override
            public void run() {
                mRootView.setRGBImageView(rgb);
            }
        });
    }

    public void setFPS(final float fps) {
        checkInit();
        mUIHandler.post(new Runnable() {
            @Override
            public void run() {
                checkInit();
                mRootView.setFPS(fps);
            }
        });
    }

    public void setMoreInformation(final String info) {
        checkInit();
        mUIHandler.post(new Runnable() {
            @Override
            public void run() {
                checkInit();
                mRootView.setMoreInformation(info);
            }
        });
    }

    private void checkInit() {
        if (mRootView == null) {
            init();
        }
    }

    @UiThread
    private final class FloatCamView extends FrameLayout {
        private WeakReference<FloatingCameraWindow> mWeakRef;
        private static final int MOVE_THRESHOLD = 10;
        private int mLastX;
        private int mLastY;
        private int mFirstX;
        private int mFirstY;
        private LayoutInflater mLayoutInflater;
        private ImageView mColorView;
        private TextView mFPSText;
        private TextView mInfoText;
        private boolean mIsMoving = false;

        public FloatCamView(FloatingCameraWindow window) {
            super(window.mContext);
            mWeakRef = new WeakReference<FloatingCameraWindow>(window);
            // mLayoutInflater = LayoutInflater.from(context);
            mLayoutInflater = (LayoutInflater) window.mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            FrameLayout body = (FrameLayout) this;
            body.setOnTouchListener(new OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    return false;
                }
            });

            View floatView = mLayoutInflater.inflate(R.layout.cam_window_view, body, true);
            mColorView = (ImageView) findViewById(R.id.imageView_c);
            mFPSText = (TextView) findViewById(R.id.fps_textview);
            mInfoText = (TextView) findViewById(R.id.info_textview);
            mFPSText.setVisibility(View.GONE);
            mInfoText.setVisibility(View.GONE);

            int colorMaxWidth = (int) (mWindowWidth* window.mScaleWidthRatio);
            int colorMaxHeight = (int) (mWindowHeight * window.mScaleHeightRatio);

            mColorView.getLayoutParams().width = colorMaxWidth;
            mColorView.getLayoutParams().height = colorMaxHeight;
        }

        @Override
        public boolean onTouchEvent(MotionEvent event) {
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    mLastX = (int) event.getRawX();
                    mLastY = (int) event.getRawY();
                    mFirstX = mLastX;
                    mFirstY = mLastY;
                    break;
                case MotionEvent.ACTION_MOVE:
                    int deltaX = (int) event.getRawX() - mLastX;
                    int deltaY = (int) event.getRawY() - mLastY;
                    mLastX = (int) event.getRawX();
                    mLastY = (int) event.getRawY();
                    int totalDeltaX = mLastX - mFirstX;
                    int totalDeltaY = mLastY - mFirstY;

                    if (mIsMoving
                            || Math.abs(totalDeltaX) >= MOVE_THRESHOLD
                            || Math.abs(totalDeltaY) >= MOVE_THRESHOLD) {
                        mIsMoving = true;
                        WindowManager windowMgr = mWeakRef.get().mWindowManager;
                        WindowManager.LayoutParams parm = mWeakRef.get().mWindowParam;
                        if (event.getPointerCount() == 1 && windowMgr != null) {
                            parm.x -= deltaX;
                            parm.y -= deltaY;
                            windowMgr.updateViewLayout(this, parm);
                        }
                    }
                    break;

                case MotionEvent.ACTION_UP:
                    mIsMoving = false;
                    break;
            }
            return true;
        }

        public void setRGBImageView(Bitmap rgb) {
            if (rgb != null && !rgb.isRecycled()) {
                mColorView.setImageBitmap(rgb);
            }
        }

        public void setFPS(float fps) {
            if (mFPSText != null) {
                if (mFPSText.getVisibility() == View.GONE) {
                    mFPSText.setVisibility(View.VISIBLE);
                }
                mFPSText.setText(String.format("FPS: %.2f", fps));
            }
        }

        public void setMoreInformation(String info) {
            if (mInfoText != null) {
                if (mInfoText.getVisibility() == View.GONE) {
                    mInfoText.setVisibility(View.VISIBLE);
                }
                mInfoText.setText(info);
            }
        }
    }

}
