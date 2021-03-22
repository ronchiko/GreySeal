package com.roncho.greyseal.engine;

public class Vector2 {
    public float x, y;

    public Vector2(float x, float y){
        this.x = x;
        this.y = y;
    }
    public Vector2(Vector2 o){
        this(o.x, o.y);
    }
    public Vector2(){
        this(0, 0);
    }

    public Vector2 add(Vector2 b){
        return new Vector2(x + b.x, y + b.y);
    }
    public Vector2 sub(Vector2 b){
        return new Vector2(x - b.x, y - b.y);
    }
    public Vector2 inv(){
        return new Vector2(-x, -y);
    }
    public Vector2 swap(){
        return new Vector2(y, x);
    }
    public Vector2 scale(float s){
        return new Vector2(x * s, y * s);
    }

    @Override
    public String toString() {
        return "(" + x + ", " + y + ")";
    }
}
