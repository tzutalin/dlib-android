/*
 * Copyright 2016 Tzutalin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.tzutalin.dlibtest;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.support.annotation.NonNull;
import android.util.AttributeSet;
import android.util.TypedValue;
import android.view.View;

public class TrasparentTitleView extends View {
    private static final float TEXT_SIZE_DIP = 24;
    private static final String TAG = "RecognitionScoreView";
    private String mShowText;
    private final float mTextSizePx;
    private final Paint mFgPaint;
    private final Paint mBgPaint;

    public TrasparentTitleView(final Context context, final AttributeSet set) {
        super(context, set);

        mTextSizePx =
                TypedValue.applyDimension(
                        TypedValue.COMPLEX_UNIT_DIP, TEXT_SIZE_DIP, getResources().getDisplayMetrics());
        mFgPaint = new Paint();
        mFgPaint.setTextSize(mTextSizePx);

        mBgPaint = new Paint();
        mBgPaint.setColor(0xcc4285f4);
    }

    @NonNull
    public void setText(@NonNull String text) {
        this.mShowText = text;
        postInvalidate();
    }

    @Override
    public void onDraw(final Canvas canvas) {
        final int x = 10;
        int y = (int) (mFgPaint.getTextSize() * 1.5f);

        canvas.drawPaint(mBgPaint);

        if (mShowText != null) {
            canvas.drawText(mShowText, x, y, mFgPaint);
        }
    }
}
