package com.roncho.greyseal.engine;

import android.util.Log;

import java.util.Objects;

public final class SealLog {
    public static void Log(String msg){
        Log.d("[Log] Grey Seal", msg);
    }
    public static void Error(String msg) {
        Log.e("[Error] Grey Seal", msg);
    }
    public static void Exception(Exception e) {
        Log.e("[Error] Grey Seal", Objects.requireNonNull(e.getMessage()));
    }
    public static void Warning(String msg){
        Log.w("[Warn] Grey Seal", msg);
    }
}
