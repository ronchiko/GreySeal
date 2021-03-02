package com.roncho.greyseal.engine.android;

import android.opengl.GLSurfaceView;

import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.engine.systems.stream.SealObjectStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class SealRenderer implements GLSurfaceView.Renderer {

    private static native void init(int width, int height, byte[] array);
    private static native void step(byte[] updatedData, byte[] engineCommands);
    private static native byte[] requestEngineData();

    public SealRenderer(){

    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int w, int h) {
        init(w, h, SealSystemManager.getEngineCalls());
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        // getSealObjects();
        SealObjectStream stream = new SealObjectStream(requestEngineData());

        SealSystemManager.runSystems(stream);
        byte[] instructions = SealSystemManager.getEngineCalls();

        step(stream.getRaw(), instructions);
    }
}
