package com.roncho.greyseal.engine;

import android.util.DisplayMetrics;

import androidx.appcompat.app.AppCompatActivity;

public final class SealMetrics {

    private static int screenWidth, screenHeight, densityDpi;
    private static float xdpi, ydpi, density;

    static void reload(AppCompatActivity activity){
        DisplayMetrics displayMetrics = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        screenWidth = displayMetrics.widthPixels;
        screenHeight = displayMetrics.heightPixels;
        densityDpi = displayMetrics.densityDpi;
        xdpi = displayMetrics.xdpi;
        ydpi = displayMetrics.ydpi;
        density = displayMetrics.density;
    }


    public static int getScreenWidth () { return screenWidth;  }
    public static int getScreenHeight() { return screenHeight; }
    public static int getDensityDpi  () { return densityDpi;   }
    public static float getXdpi() { return xdpi; }
    public static float getYdpi() { return ydpi; }
    public static float getDensity() { return density; }

    public static float percentageY(float y){ return y / screenHeight; }
    public static float percentageX(float x){ return x / screenWidth ; }
}
