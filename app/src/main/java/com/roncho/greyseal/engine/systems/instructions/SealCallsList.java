package com.roncho.greyseal.engine.systems.instructions;

import com.roncho.greyseal.engine.android.cpp.SealCppHandler;

import java.nio.ByteBuffer;

public class SealCallsList {

    private static class SealCallNode {
        private SealCallType type;
        private byte[] operands;
        private SealCallNode next;
    }

    private int finalBufferSize;
    private SealCallNode head, tail;

    public SealCallsList(){
        head = tail = null;
        finalBufferSize = 0;
    }

    public void queueCall(SealCallType type, ByteBuffer operands){

        SealCallNode node = new SealCallNode();
        node.next = null;
        node.operands = SealCppHandler.arrayCpp(operands);
        node.type = type;
        finalBufferSize += node.operands.length + 1; // The amount of used bytes + instruction code

        if(head == null){
            head = tail = node;
        }else{
            tail = (tail.next = node);
        }
    }

    public byte[] compile(){
        byte[] buffer = new byte[finalBufferSize];
        int i = 0;
        SealCallNode node = head;
        while(node != null){
            buffer[i++] = node.type.getCode();
            for(int j =0; j < node.operands.length; j++)
                buffer[i++] = node.operands[j];
            node = node.next;
        }
        return buffer;
    }

    public void reset(){
        head = tail = null;
        finalBufferSize = 0;
    }
}
