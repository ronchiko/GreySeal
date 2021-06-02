package com.roncho.greyseal.engine.api;

import com.roncho.greyseal.engine.SealEngine;

public class Utilities {
    /**
     * Gets the extension of a path
     * @param path
     * @return
     */
    public static String getPathExtension(String path){
        int last = path.lastIndexOf('.');
        if(last < 0) return "";     // No extension
        return path.substring(last + 1);
    }

    public static AssetFileType getAssetType(String path){
        return AssetFileType.matchForExtension(getPathExtension(path));
    }

    public static int loadFont(HasStringValue path) {
        return SealEngine.loadFont(path);
    }
    public static int loadFont(String path) { return SealEngine.loadFont(path); }
}
