package com.roncho.greyseal.engine.android;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Typeface;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.util.logging.Logger;

public final class SealTexturePipeline {

    private static AssetManager assets;

    private final static String _Chars = syncCharset();

    public static void init(AssetManager a){
        assets = a;
    }

    private static byte[] decodeBMP(Bitmap e){

        final int width = e.getWidth(), height = e.getHeight();

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
        return array;
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
            byte[] array = decodeBMP(e);

            return registerBmp(array, width, height);
        }catch (IOException e){
            Log.e("Greyseal", "Failed to load texture " + path);
        }
        return -1;
    }

    public static int loadFont(String path, float size){
        Typeface font = Typeface.createFromAsset(assets, path);

        if(font == null) return -1;

        Paint textPainter = new Paint();
        textPainter.setTextSize(size);
        textPainter.setFakeBoldText(false);
        textPainter.setAntiAlias(true);
        textPainter.setTypeface(font);

        textPainter.setARGB(255, 255, 255, 255);

        textPainter.setSubpixelText(true);
        textPainter.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.SCREEN));

        float textWidth = textPainter.measureText(_Chars);

        final int textureWidth = (int)(textWidth + 2);
        final int textureHeight = (int)size + 2;

        Bitmap texture = Bitmap.createBitmap(textureWidth, textureHeight, Bitmap.Config.ARGB_8888);
        texture.eraseColor(Color.argb(0, 255, 255, 255));

        Canvas canvas = new Canvas(texture);
        canvas.drawText(_Chars, 1, 1 + size * .75f, textPainter);

        return registerFont(decodeBMP(texture), textureWidth, textureHeight, size);
    }

    private static native String syncCharset();
    private static native int registerFont(byte[] raw, int w, int h, float fs);
    private static native int registerBmp(byte[] raw, int w, int h);
}
