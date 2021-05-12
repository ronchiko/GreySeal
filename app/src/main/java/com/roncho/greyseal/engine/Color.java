package com.roncho.greyseal.engine;

public class Color {

    public static int fromRGBA(int r, int g, int b, int a){
        return ((r & 0xFF)) | ((g & 0xFF) << 8) | ((b & 0xFF) << 16) | ((a & 0xFF) << 24);
    }
}
