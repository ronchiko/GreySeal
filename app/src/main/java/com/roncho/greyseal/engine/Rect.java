package com.roncho.greyseal.engine;

public class Rect {

    public float x, y, w, h;

    public Rect(Vector2 position, Vector2 size){
        this(position.x, position.y, size.x, size.y);
    }

    public Rect(float x, float y, float w, float h){
        this.x = x;
        this.y = y;
        this.w = w;
        this.h = h;
    }

    /**
     * <p> The rightmost X of the rectangle </p>
     */
    public float endX() { return x + w; }
    /**
     * <p> The most bottom Y of the rectangle </p>
     */
    public float endY() { return y + h; }
}
