package com.roncho.greyseal.engine.android.cpp;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class SealCppHandler {

    private final static ByteOrder cppByteOrder = testByteOrder();
    private final static ByteOrder nativeByteOrder = ByteOrder.nativeOrder();

    private static ByteOrder testByteOrder(){
        return JNIGet1()[0] == 0x10 ? ByteOrder.LITTLE_ENDIAN : ByteOrder.BIG_ENDIAN;
    }

    /**
     * Takes a java.nio.ByteBuffer and sorts it to the C++ endianness and puts the result to an array
     * @param buffer The buffer to put to array
     * @return The result array
     */
    public static byte[] arrayCpp(ByteBuffer buffer){
        byte[] array = new byte[buffer.position()];
        projectCpp(array, buffer, 0, array.length);
        return array;
    }

    /**
     * Projects a java.nio.ByteBuffer into a byte[] in the C++ endianness
     * @param array The array to project to
     * @param buffer The buffer to project from
     * @param start The start of the overriding
     * @param amount The amount of bytes to project
     */
    public static void projectCpp(byte[] array, ByteBuffer buffer, int start, int amount){
        buffer.order(cppByteOrder);
        buffer.position(0);
        for(int i = 0; i < amount; i++)
            array[start + i] = buffer.get();
    }

    public static ByteBuffer allocateJava(int size){
        return ByteBuffer.allocate(size).order(nativeByteOrder);
    }
    public static ByteBuffer allocateJava(byte[] array, int start, int end){
        return ByteBuffer.wrap(array, start, end).order(nativeByteOrder);
    }

    private static native byte[] JNIGet1();
}
