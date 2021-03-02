package com.roncho.greyseal.engine.systems.instructions;

public enum SealCallType {
    NOP(0),                  // NOP
    INSTANTIATE_0(1),        // SNT str v3 qt v3
    INSTANTIATE_1(2),        // SNT str v3
    INSTANTIATE_2(3),        // SNT str
    DESTROY(4),              // DESTROY oid
    LOAD_TEXTURE(5),         // LDT str
    LOAD_MATERIAL(6),        // LDS str str
    LOAD_MESH(7),            // LDM str
    CLONE_1(8),              // CLN oid
    CLONE_2(9),              // CLN oid v3 qt v3
    CLONE_3(10);             // CLN oid v3


    private final byte code;

    private SealCallType(int code){
        this.code = (byte)code;
    }

    public byte getCode() { return code; }
}
