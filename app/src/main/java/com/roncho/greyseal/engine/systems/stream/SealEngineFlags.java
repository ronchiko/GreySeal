package com.roncho.greyseal.engine.systems.stream;

public enum SealEngineFlags {
    DESTROY(0x1),
    NEW(0x2),
    DONT_DRAW(0x4),
    DESTROYED(0x8);

    final byte value, mask;

    SealEngineFlags(int value) { this.value = (byte)value; this.mask = (byte)~value; }
}
