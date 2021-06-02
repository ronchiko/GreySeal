package com.roncho.greyseal.engine;

public class Color {

    public static int fromRGBA(int r, int g, int b, int a){
        return ((r & 0xFF)) | ((g & 0xFF) << 8) | ((b & 0xFF) << 16) | ((a & 0xFF) << 24);
    }

    public static boolean compareRGB(int rgba1, int rgba2){
        return ((rgba1 ^ rgba2) & 0xFFFFFF) == 0;
    }

    public static int alpha(int c){
        return (c & 0xFF000000) >> 24;
    }
}
