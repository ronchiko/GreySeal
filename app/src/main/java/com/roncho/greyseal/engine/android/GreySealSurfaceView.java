package com.roncho.greyseal.engine.android;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

public class GreySealSurfaceView extends GLSurfaceView {

    private GreySealRenderer renderer;

    public GreySealSurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(2);
        renderer = new GreySealRenderer();
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
