package com.roncho.greyseal.engine.android;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import com.roncho.greyseal.engine.TouchInput;

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

        if(event.getAction() == MotionEvent.ACTION_DOWN){
            sendSealTouchEvent(event.getX(), event.getY());
            TouchInput.touchEvent(event.getX(), event.getY());
        }else if(event.getAction() == MotionEvent.ACTION_UP){
            TouchInput.stopTouching();
        }

        return super.onTouchEvent(event);
    }

    private native void sendSealTouchEvent(float x, float y);
}
