package com.roncho.greyseal.engine.systems.stream;

public final class EntityStream {

    // The size (in bytes) of a single object
    final static int SIZEOF_OBJECT = getSizeofNativeObject();

    private int current;
    private final int count;
    private final byte[] stream;

    public EntityStream(byte[] stream) {
        current = 0;
        count = stream.length / SIZEOF_OBJECT;
        this.stream = stream;
    }

    public boolean hasNext(){
        return current < count;
    }

    public Entity next(){
        return new Entity(stream, (current++) * SIZEOF_OBJECT);
    }

    public byte[] getRaw() { return stream; }

    public void write(Entity e){
        e.write(stream);
    }
    public Entity at(int index) { return new Entity(stream, index * SIZEOF_OBJECT); }
    private static native int getSizeofNativeObject();
}
