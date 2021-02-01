package com.roncho.greyseal.engine.systems.instructions;

public enum SealCallType {
    SEAL_INSTANTIATE_0((byte)1, 14),
    SEAL_DESTROY((byte)2, 2);

    private final byte code;
    private final int nextBytes;

    private SealCallType(byte code, int nextBytes){
        this.code = code;
        this.nextBytes = nextBytes;
    }

    public byte getCode() { return code; }
    public int getNextBytes() { return nextBytes; }
}
