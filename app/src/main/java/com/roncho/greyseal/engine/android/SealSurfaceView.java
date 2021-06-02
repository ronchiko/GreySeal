package com.roncho.greyseal.engine.android;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.DragEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;

import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.systems.SealSystemManager;

public final class SealSurfaceView extends GLSurfaceView {

    private SealRenderer renderer;

    public SealSurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(2);
        setPreserveEGLContextOnPause(true);
        renderer = new SealRenderer();
        setRenderer(renderer);

        // Instantiate the base input class
        Input.instance = new Input();
        setOnTouchListener((View v, MotionEvent e) -> {
            // Every event we get an MotionEvent class object, which contains the data of current action,
            // Each action contains a bunch of pointers and each pointer has an id, so in the input class
            // we can save the pointer input
            performClick();
            Input.instance.enqueue(e);
            return true;
        });
    }
}
