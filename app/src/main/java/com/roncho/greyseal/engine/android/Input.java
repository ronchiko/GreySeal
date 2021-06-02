package com.roncho.greyseal.engine.android;

import android.annotation.SuppressLint;
import android.view.MotionEvent;

import androidx.annotation.NonNull;

import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.Vector2;

import java.util.ArrayList;
import java.util.Iterator;

public final class Input {

    private static final int INCREMENT_STEP = 10;

    public enum TouchState {
        INVALID(-1),
        DOWN(MotionEvent.ACTION_DOWN),
        UP(MotionEvent.ACTION_UP),
        MOVE(MotionEvent.ACTION_MOVE);

        final int action;

        TouchState(int aid){
            action = aid;
        }
    }

    /**
     * When caching touches make sure to check if they are valid, and dispose of them if they are not!
     */
    public static class Touch {
        private TouchState state;
        private final int pointerId;
        private final Vector2 origin, current, last;

        public Touch(int pid){
            origin = new Vector2();
            current = new Vector2();
            last = new Vector2();
            state = TouchState.INVALID;
            pointerId = pid;
        }
        private void position(float x, float y){
            last.x = current.x; last.y = current.y;
            current.x = x; current.y = y;
        }
        private void start(float x, float y){
            current.x = origin.x = last.x = x;
            current.y = origin.y = last.y = y;
        }

        // Interface methods
        public boolean isValid() { return state != TouchState.INVALID; }
        public boolean isDown() { return state != TouchState.DOWN; }

        public float x() { return current.x; }
        public float y() { return current.y; }

        public float startX() { return origin.x; }
        public float startY() { return origin.y; }

        public float deltaX() { return current.x - last.x; }
        public float deltaY() { return current.y - last.y; }

        public final Vector2 startVector() { return origin; }

        @SuppressLint("DefaultLocale")
        public String toString(){
            return "[" + state + "]" + origin + ":" + last + "->" + current;
        }
    }

    private Touch[] touches;
    private int size, count;
    private ArrayList<MotionEvent> m_QueuedMotionEvents;

    static Input instance;


    Input(){
        size = count = 0;
        touches = new Touch[0];
        m_QueuedMotionEvents = new ArrayList<>();
    }

    void enqueue(MotionEvent e){
        m_QueuedMotionEvents.add(e);
    }

    void remove(){
        for(int i = 0; i < count; i++){
            if(touches[i] != null && touches[i].state == TouchState.INVALID){
                System.arraycopy(touches, i, touches, i + 1, count - i - 1);
                count--;
                return;
            }
        }
    }

    void updateOnce(){
        for(MotionEvent e : m_QueuedMotionEvents) {
            // Only works for touches, not for all the other wheels and stuff
            for (int i = 0; i < e.getPointerCount(); i++) {
                Touch t = get(e.getPointerId(i));
                switch (e.getActionMasked()) {
                    case MotionEvent.ACTION_POINTER_DOWN:
                    case MotionEvent.ACTION_DOWN:
                        t.start(e.getX(i), e.getY(i));
                        t.state = TouchState.DOWN;
                        break;
                    case MotionEvent.ACTION_POINTER_UP:
                    case MotionEvent.ACTION_UP:
                    case MotionEvent.ACTION_CANCEL:
                        t.state = TouchState.INVALID;
                        break;
                    case MotionEvent.ACTION_MOVE:
                        t.state = TouchState.MOVE;
                        t.position(e.getX(i), e.getY(i));
                        break;
                }
            }
        }
        m_QueuedMotionEvents.clear();
    }
    private Touch get(int pid){
        for(int i = 0; i < count; i++){
            if(touches[i] != null && touches[i].pointerId == pid) return touches[i];
        }
        // Add a new touch
        if(size <= count){
            Touch[] temp = new Touch[size + INCREMENT_STEP];
            System.arraycopy(temp, 0, touches, 0, count);
            touches = temp;
            size += INCREMENT_STEP;
        }
        return touches[count++] = new Touch(pid);
    }

    public static class TouchArray implements Iterable<Touch> {
        private Touch[] array;

        TouchArray(Touch[] array){
            this.array = array;
        }


        @NonNull
        @Override
        public Iterator<Touch> iterator() {
            return new Iterator<Touch>() {

                private int i = 0;

                @Override
                public boolean hasNext() {
                    return i >= 0 && i < array.length;
                }

                @Override
                public Touch next() {
                    return array[i++];
                }
            };
        }
    }

    public static TouchArray touches() { return new TouchArray(instance.touches); }
    public static int touchCount() { return instance.count; }
    public static Touch touch(int index) { return instance.touches[index]; }
}
