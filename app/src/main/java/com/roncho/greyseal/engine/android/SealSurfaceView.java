package com.roncho.greyseal.engine.android;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class SealSurfaceView extends GLSurfaceView {

    private SealRenderer renderer;

    public SealSurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(2);
        renderer = new SealRenderer();
        setRenderer(renderer);
    }

    @Override
    public boolean performClick() {
        return super.performClick();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        performClick();
        sendSealTouchEvent(event.getX(), event.getY());
        return super.onTouchEvent(event);
    }

    private native void sendSealTouchEvent(float x, float y);
}
