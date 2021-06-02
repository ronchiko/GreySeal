package com.roncho.greyseal.engine.api;

public enum AssetFileType {
    ENTITY_FILE,
    MODEL_FILE,
    FONT_FILE,
    SHADER_FILE,
    IMAGE_FILE,
    UNKNOWN;

    public static AssetFileType matchForExtension(String extension){
        String e = extension.toLowerCase();
        if(e.equals("ntt")) return ENTITY_FILE;
        if(e.equals("obj")) return MODEL_FILE;
        if(e.equals("ttf")) return FONT_FILE;
        if(e.equals("glsl") || e.equals("vert") || e.equals("frag")) return SHADER_FILE;
        if(e.equals("jpg") || e.equals("png") || e.equals("bmp") || e.equals("jpeg")) return IMAGE_FILE;
        return UNKNOWN;
    }
}
