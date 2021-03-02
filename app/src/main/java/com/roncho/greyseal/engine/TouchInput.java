package com.roncho.greyseal.engine;

import android.util.Log;

public class TouchInput {

    private static float _lastX, _lastY;
    private static boolean _touching;

    public static boolean isTouching() { return _touching; }

    public static void touchEvent(float x, float y) {
        //Log.d("Seal", "Touching screen");
        _lastX = x; _lastY = y;
        _touching = true;
    }

    public static void stopTouching(){
        _touching = false;
    }

    public static float x() { return _lastX; }
}
