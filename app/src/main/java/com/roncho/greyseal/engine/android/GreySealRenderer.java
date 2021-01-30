package com.roncho.greyseal.engine.android;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GreySealRenderer implements GLSurfaceView.Renderer {

    private static native void init(int width, int height);
    private static native void step();

    public GreySealRenderer(){

    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int w, int h) {
        init(w, h);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        step();
    }
}
