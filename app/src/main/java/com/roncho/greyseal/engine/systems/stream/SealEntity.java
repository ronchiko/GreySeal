package com.roncho.greyseal.engine.systems.stream;

import com.roncho.greyseal.engine.Quaternion;
import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.android.cpp.SealCppHandler;

import java.nio.ByteBuffer;

public class SealEntity {

    private static native int getObjectPayloadSize();

    private static final int USER_FLAGS_SIZE = 4;
    private static final int ENGINE_FLAGS_SIZE = 1, ENGINE_FLAGS_START = USER_FLAGS_SIZE;
    private static final int VECTOR3_SIZE = 12, QUATERNION_SIZE = 16, HANDLE_SIZE = 4, COLOR_SIZE = 4;
    private static final int POSITION_START = ENGINE_FLAGS_START + ENGINE_FLAGS_SIZE;
    private static final int ROTATION_START = POSITION_START + VECTOR3_SIZE;
    private static final int SCALE_START = ROTATION_START + QUATERNION_SIZE;
    private static final int COLOR_START = SCALE_START + VECTOR3_SIZE;
    private static final int MATERIAL_START = COLOR_START + COLOR_SIZE;
    private static final int TEXTURE_START = MATERIAL_START + HANDLE_SIZE;
    private static final int MESH_START = TEXTURE_START + HANDLE_SIZE;

    // Not used
    private static final int PAYLOAD_START = MESH_START + HANDLE_SIZE;
    private static final int PAYLOAD_SIZE = getObjectPayloadSize();

    private static final int BUFFER_SIZE = PAYLOAD_SIZE + PAYLOAD_START;

    private final int start;
    public int userFlags;
    public final short engineFlags, uid;

    public Vector3 position;
    public Quaternion rotation;
    public Vector3 scale;
    public int color, material, texture, mesh;

    public SealEntity(byte[] array, int start){
        this.start = start;

        ByteBuffer bb = SealCppHandler.allocateJava(array, start, SealObjectStream.SIZEOF_OBJECT);

        userFlags = bb.getInt();
        engineFlags = bb.getShort();
        uid = bb.getShort();

        position = Vector3.readFromBuffer(bb);
        rotation = Quaternion.readFromBuffer(bb);
        scale = Vector3.readFromBuffer(bb);

        color = bb.getInt();
        material = bb.getInt();
        texture = bb.getInt();
        mesh = bb.getInt();
    }

    public void write(byte[] buffer){
        ByteBuffer bb = SealCppHandler.allocateJava(SealObjectStream.SIZEOF_OBJECT);
        bb.position(0);

        bb.putInt(userFlags);
        bb.putShort(engineFlags);
        bb.putShort(uid);

        position.writeToBuffer(bb);
        rotation.writeToBuffer(bb);
        scale.writeToBuffer(bb);

        bb.putInt(color).putInt(material).putInt(texture).putInt(mesh);
        SealCppHandler.projectCpp(buffer, bb, start, SealObjectStream.SIZEOF_OBJECT);
    }

    public short localIndex(){
        return (short)(start / SealObjectStream.SIZEOF_OBJECT);
    }
}
