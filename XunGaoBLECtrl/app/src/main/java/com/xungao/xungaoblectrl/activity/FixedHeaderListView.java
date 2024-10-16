package com.xungao.xungaoblectrl.activity;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ListView;

/**
 * Created by lucelu on 2024/10/14.
 */

public class FixedHeaderListView extends ListView {
    private View headerView;

    public FixedHeaderListView(Context context) {
        super(context);
    }

    public FixedHeaderListView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public FixedHeaderListView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public void setFixedHeader(View headerView) {
        this.headerView = headerView;
    }

    @Override
    protected void dispatchDraw(Canvas canvas) {
        if (headerView != null) {
            // Draw the header first
            canvas.save();
            int top = 0;
            canvas.translate(0, top);
            headerView.draw(canvas);
            canvas.restore();
        }

        // Then draw the list view's content
        super.dispatchDraw(canvas);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        if (headerView != null) {
            // Account for the header view
            int height = getResources().getDisplayMetrics().heightPixels;
            headerView.measure(widthMeasureSpec, View.MeasureSpec.makeMeasureSpec(height, View.MeasureSpec.AT_MOST));
            heightMeasureSpec = MeasureSpec.makeMeasureSpec(height - headerView.getMeasuredHeight(), MeasureSpec.AT_MOST);
        }
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }
}
