package com.roncho.greyseal.engine;

public class Tuple<T1, T2> {
    private T1 first;
    private T2 second;

    public Tuple(T1 t1, T2 t2){
        first = t1; second = t2;
    }

    public T1 getFirst() { return first; }
    public T2 getSecond() { return second; }

    public void setFirst(T1 first) {
        this.first = first;
    }
    public void setSecond(T2 second) {
        this.second = second;
    }
}
