package com.roncho.greyseal.engine.systems.stream;

import com.roncho.greyseal.engine.SealCamera;
import com.roncho.greyseal.engine.SealLog;

public class SealObjectStream {

    // The size (in bytes) of a single object
    final static int SIZEOF_OBJECT = getSizeofNativeObject();

    private int current;
    private final int count;
    private final byte[] stream;

    public SealObjectStream(byte[] stream) {
        current = 0;
        count = stream.length / SIZEOF_OBJECT;
        this.stream = stream;
    }

    public boolean hasNext(){
        return current < count;
    }

    public SealEntity next(){
        return new SealEntity(stream, (current++) * SIZEOF_OBJECT);
    }

    public byte[] getRaw() { return stream; }

    public void write(SealEntity e){
        e.write(stream);
    }
    public SealEntity at(int index) { return new SealEntity(stream, index * SIZEOF_OBJECT); }
    private static native int getSizeofNativeObject();
}
