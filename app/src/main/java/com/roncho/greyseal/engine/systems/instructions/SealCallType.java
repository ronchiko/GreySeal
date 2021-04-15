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
    FOR(8),                  // FOR byte INST
    CLONE_1(9),              // CLN oid
    CLONE_2(10),             // CLN oid v3 qt v3
    CLONE_3(11),
    UIO_New(12),             // UIO.New int
    UIO_Set_I(13),           // UIO.Set int int int
    UIO_Set_F(14),           // UIO.Set int int float
    UIO_Set_S(15),           // UIO.Set int int str
    UIO_Set_VM(16),          // UIO.VM int int
    UIO_Del(17),             // UIO.Del int
    LOAD_FONT(18);           // LDF str


    private final byte code;

    private SealCallType(int code){
        this.code = (byte)code;
    }

    public byte getCode() { return code; }
}
