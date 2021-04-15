package com.roncho.greyseal.engine;

public final class Time {

    private static long startTime;      // ms
    private static long frameTime;      // ms
    private static float deltaTime;     // sec

    public static void start() { startTime = frameTime = System.currentTimeMillis(); }

    public static void nextFrame() {
        final long currentMs = System.currentTimeMillis();
        final long elapsedMs = currentMs - frameTime;
        frameTime = currentMs;
        deltaTime = elapsedMs / 1000f;
    }


    public static float time() { return (System.currentTimeMillis() - startTime) / 1000f; }
    public static float deltaTime() { return deltaTime; }
}
