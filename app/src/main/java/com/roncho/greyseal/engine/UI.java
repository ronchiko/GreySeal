package com.roncho.greyseal.engine;

import com.roncho.greyseal.engine.android.cpp.SealCppHandler;
import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.engine.systems.instructions.SealCallType;

import java.nio.ByteBuffer;

public class UI {

    private static int m_Index = 0;

    public enum Alignment {
        Left(0),
        Center(1),
        Right(2);

        byte bits;

        Alignment(int bits) {this.bits = (byte)bits;}
    }
    private static Alignment getAlignmentEnum(int bits){
        switch (bits){
            case 0: return Alignment.Left;
            case 1: return Alignment.Center;
            case 2: return Alignment.Right;
        }
        return null;
    }

    private enum UioProperties {
        I_Texture(0),
        I_Font(1),
        I_Material(2),

        F_X(3),
        F_Y(4),
        F_W(5),
        F_H(6),

        I_Color(7),

        S_Text(8),

        I_TextMaterial(9),
        I_TextColor(10),
        F_TextSize(11),

        F_PadL(12),
        F_PadT(13),
        F_PadR(14),
        F_PadB(15),

        I_Flags(16);

        private final int pid;

        UioProperties(int pid) { this.pid = pid; }
    }

    public static class UIObject {
        private final int index;

        private int flags = 0;
        private int color = 0xFFFFFFFF, textColor = 0x000000FF;
        private String text = "";
        private int font = -1;
        private int texture = 0;
        private float textSize = 48;
        private int material, textMaterial;
        private final Rect transform, padding;

        private UIObject(){
            this.index = m_Index++;
            transform = new Rect(0, 0, 100, 100);
            padding = new Rect(5, 5, 5, 5);
        }

        public UIObject setText(String text){
            this.text = text;
            SetUioPropS(index, UioProperties.S_Text, text);
            return this;
        }
        public final String getText() { return text; }

        public UIObject setTransform(float x, float y, float w, float h){
            transform.x = x;
            transform.y = y;
            transform.w = w;
            transform.h = h;

            SetUioPropF(index, UioProperties.F_X, x);
            SetUioPropF(index, UioProperties.F_Y, y);
            SetUioPropF(index, UioProperties.F_W, w);
            SetUioPropF(index, UioProperties.F_H, h);
            return this;
        }
        public UIObject setTransformRelative(float x, float y, float w, float h){
            int sw = SealMetrics.getScreenWidth(), sh = SealMetrics.getScreenHeight();
            return setTransform(x * sw, y * sh, w * sw, h * sh);
        }
        // With padding, NOTE: Padding in pixels
        public UIObject setTransformRelative(float x, float y, float w, float h, float l, float t,
                                             float r, float b){
            int sw = SealMetrics.getScreenWidth(), sh = SealMetrics.getScreenHeight();
            setTransform(x * sw - l, y * sh  - t, w * sw - r - l, h * sh - t - b);
            return setPadding(l, t, r, b);
        }
        public final Rect getTransform() { return transform; }
        public UIObject setPadding(float l, float t, float r, float b){
            padding.x = l;
            padding.w = r;
            padding.y = t;
            padding.h = b;

            SetUioPropF(index, UioProperties.F_PadL, l);
            SetUioPropF(index, UioProperties.F_PadR, r);
            SetUioPropF(index, UioProperties.F_PadT, t);
            SetUioPropF(index, UioProperties.F_PadB, b);
            return this;
        }
        public final Rect getPadding() { return padding; }

        public UIObject setFont(int fontIndex){
            font = fontIndex;
            SetUioPropI(index, UioProperties.I_Font, fontIndex);
            return this;
        }
        public final int getFont() { return font; }

        public UIObject setColor(int r, int g, int b, int a){
            color = Color.fromRGBA(r, g, b, a);
            SetUioPropI(index, UioProperties.I_Color, color);
            return this;
        }
        public final int getColor() { return color; }
        public UIObject setTextColor(int r, int g, int b, int a){
            textColor = Color.fromRGBA(a, b, g, r);
            SetUioPropI(index, UioProperties.I_TextColor, textColor);
            return this;
        }
        public final int getTextColor() { return textColor; }
        /** Make the background of this UI transparent */
        public final UIObject makeTransparent() { return setColor(255, 255, 255, 0);}
        public void setBackgroundImage(int texture) {
            this.texture = texture;
            SetUioPropI(index, UioProperties.I_Texture, texture);
        }
        public final int getBackgroundImage() { return texture;}

        public UIObject setTextXAlignment(Alignment alignment){
            flags = flags & ~0x3 | alignment.bits;
            SetUioPropI(index, UioProperties.I_Flags, flags);
            return this;
        }
        public UIObject setTextYAlignment(Alignment alignment){
            flags = flags & ~0xC | alignment.bits;
            SetUioPropI(index, UioProperties.I_Flags, flags);
            return this;
        }
        public UIObject setTextAlignment(Alignment xAlign, Alignment yAlign){
            return setTextXAlignment(xAlign).setTextYAlignment(yAlign);
        }
        public final Alignment getTextYAlignment() {
            return getAlignmentEnum(flags >> 2 & 0x3);
        }
        public final Alignment getTextXAlignment() {
            return getAlignmentEnum(flags & 0x3);
        }
        public UIObject setTextSize(float size){
            textSize = size;
            SetUioPropF(index, UioProperties.F_TextSize, size);
            return this;
        }
        public float getTextSize() { return textSize; }

        public void delete(){
            DeleteUio(index);
        }
    }
    
    private static void SetUioPropS(int index, UioProperties prop, String s){
        ByteBuffer bb = SealCppHandler.allocateJava(9 + s.length());
        bb.putInt(index).putInt(prop.pid);
        SealCppHandler.putString(bb, s);
        SealSystemManager.queue(SealCallType.UIO_Set_S, bb);
    }
    private static void SetUioPropF(int index, UioProperties prop, float f){
        ByteBuffer bb = SealCppHandler.allocateJava(12);
        bb.putInt(index).putInt(prop.pid).putFloat(f);
        SealSystemManager.queue(SealCallType.UIO_Set_F, bb);
    }
    private static void SetUioPropI(int index, UioProperties prop, int i){
        ByteBuffer bb = SealCppHandler.allocateJava(12);
        bb.putInt(index).putInt(prop.pid).putInt(i);
        SealSystemManager.queue(SealCallType.UIO_Set_I, bb);
    }

    private static void DeleteUio(int index){
        SealSystemManager.queue(SealCallType.UIO_Del, SealCppHandler.allocateJava(4).putInt(index));
    }

    public static UIObject newUiObject(){
        UIObject uio = new UIObject();
        SealSystemManager.queue(SealCallType.UIO_New, SealCppHandler.allocateJava(4).putInt(uio.index));
        return uio;
    }
}
