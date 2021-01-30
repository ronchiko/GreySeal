package com.roncho.greyseal.engine.android;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.util.logging.Logger;

public class GreySealTexturePipeline {

    private static AssetManager assets;

    public static void init(AssetManager a){
        assets = a;
    }

    /**
     * Loads a texture (Not from the texture cache)
     * @param path the path to the texture to be loaded
     * @return the GL id of the loaded texture
     */
    public static int load(String path){
        try(InputStream stream = assets.open(path)){
            Bitmap e = BitmapFactory.decodeStream(stream);
            int width = e.getWidth(), height = e.getHeight();
            byte[] array = new byte[width * height * 4];

            for(int y = 0; y < height; y++) {
                for(int x = 0; x < width; x++){
                    int pixel = e.getPixel(x, y);

                    byte r = (byte)Color.red(pixel);
                    byte g = (byte)Color.green(pixel);
                    byte b = (byte)Color.blue(pixel);
                    byte a = (byte)Color.alpha(pixel);

                    array[(x + y * width) * 4] = r;
                    array[(x + y * width) * 4 + 1] = g;
                    array[(x + y * width) * 4 + 2] = b;
                    array[(x + y * width) * 4 + 3] = a;
                }
            }

            return registerBmp(array, width, height);
        }catch (IOException e){
            Log.e("Greyseal", "Failed to load texture -1");
        }
        return -1;
    }

    private static native int registerBmp(byte[] raw, int w, int h);
}
