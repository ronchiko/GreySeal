package com.roncho.greyseal.engine.systems.stream;

import com.roncho.greyseal.engine.Quaternion;
import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.android.cpp.SealCppHandler;
import com.roncho.greyseal.engine.api.MaskEnum;

import java.nio.ByteBuffer;

public final class Entity {

    private static native int getObjectPayloadSize();

    private final int start;
    public int userFlags;
    public short engineFlags;
    public final short uid;

    public Vector3 position;
    public Quaternion rotation;
    public Vector3 scale;
    public int color, material, texture, mesh;

    final long m_GeneratedOnStreamId;

    Entity(byte[] array, int start){
        this.start = start;

        ByteBuffer bb = SealCppHandler.allocateJava(array, start, EntityStream.SIZEOF_OBJECT);

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

        m_GeneratedOnStreamId = EntityStream.g_CurrentStreamId;
    }

    void write(byte[] buffer){
        ByteBuffer bb = SealCppHandler.allocateJava(EntityStream.SIZEOF_OBJECT);
        bb.position(0);

        bb.putInt(userFlags);
        bb.putShort(engineFlags);
        bb.putShort(uid);

        position.writeToBuffer(bb);
        rotation.writeToBuffer(bb);
        scale.writeToBuffer(bb);

        bb.putInt(color).putInt(material).putInt(texture).putInt(mesh);
        SealCppHandler.projectCpp(buffer, bb, start, EntityStream.SIZEOF_OBJECT);
    }

    public boolean check(SealEngineFlags flag) { return (engineFlags & flag.value) != 0; }
    public void activate(SealEngineFlags flag) { engineFlags |= flag.value; }
    public void deactivate(SealEngineFlags flag) { engineFlags &= flag.mask; }

    public boolean matches(int mask) { return (userFlags & mask) != 0; }
    public boolean matches(MaskEnum mask) { return (userFlags & mask.getMask()) != 0; }

    /**
     * Checks if an object is queued for destruction.
     * @return
     */
    public boolean queued() { return check(SealEngineFlags.DESTROYED) || check(SealEngineFlags.DESTROY);}
}
