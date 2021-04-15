package com.roncho.greyseal.engine;

import com.roncho.greyseal.engine.android.cpp.SealCppHandler;
import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.engine.systems.instructions.SealCallType;

import java.nio.ByteBuffer;

public class UI {

    private static int m_Index = 0;

    private enum UioProperties {
        I_Texture(0),
        I_Font(1),
        I_Material(2),

        F_X(3),
        F_Y(4),
        F_W(5),
        F_H(6),

        I_Color(7),

        S_Text(8);

        private final int pid;

        UioProperties(int pid) { this.pid = pid; }
    }

    public static class UIObject {
        private final int index;

        private int texture = 0, font = 0;
        private final Rect transform;
        private int material;
        private int vMode;
        private String text;

        private UIObject(){
            this.index = m_Index++;
            transform = new Rect(0, 0, 100, 100);
        }

        public void setText(String text){
            this.text = text;
            SetUioPropS(index, UioProperties.S_Text.pid, text);
        }
        public final String getText() { return text; }

        public void setTransform(float x, float y, float w, float h){
            transform.x = x;
            transform.y = y;
            transform.w = w;
            transform.h = h;

            SetUioPropF(index, UioProperties.F_X.pid, x);
            SetUioPropF(index, UioProperties.F_Y.pid, y);
            SetUioPropF(index, UioProperties.F_W.pid, w);
            SetUioPropF(index, UioProperties.F_H.pid, h);
        }
        public final Rect getTransform() { return transform; }

        public void delete(){
            DeleteUio(index);
        }


    }
    
    private static void SetUioPropS(int index, int prop, String s){
        ByteBuffer bb = SealCppHandler.allocateJava(9 + s.length());
        bb.putInt(index).putInt(prop);
        SealCppHandler.putString(bb, s);
        SealSystemManager.queue(SealCallType.UIO_Set_S, bb);
    }
    private static void SetUioPropF(int index, int prop, float f){
        ByteBuffer bb = SealCppHandler.allocateJava(12);
        bb.putInt(index).putInt(prop).putFloat(f);
        SealSystemManager.queue(SealCallType.UIO_Set_F, bb);
    }
    private static void SetUioPropI(int index, int prop, int i){
        ByteBuffer bb = SealCppHandler.allocateJava(12);
        bb.putInt(index).putInt(prop).putInt(i);
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
