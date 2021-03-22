package com.roncho.greyseal.engine.android;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.DragEvent;
import android.view.MotionEvent;
import android.view.View;

import com.roncho.greyseal.engine.SealLog;

public class SealSurfaceView extends GLSurfaceView {

    private final SealRenderer renderer;

    public SealSurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(2);
        renderer = new SealRenderer();
        setRenderer(renderer);
        Input.instance = new Input();
        setOnTouchListener((View v, MotionEvent e) -> {
            /*
             Every event we get an MotionEvent class object, which contains the data of current action,
             Each action contains a bunch of pointers and each pointer has an id, so in the input class
             we can save the pointer input
             */
            performClick();
            Input.instance.enqueue(e);
            return true;
        });
    }
}
