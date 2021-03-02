package com.roncho.greyseal.engine.systems.test;

import android.util.Log;

import com.roncho.greyseal.engine.SealEngine;
import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.engine.systems.stream.SealEntity;

public class RotateSystem extends SealSystem {

    private float a;

    @Override
    public boolean selectObject(SealEntity entity) {
        return (entity.userFlags & 0x1) != 0;
    }

    @Override
    public void updateOnce() {
        super.updateOnce();
        a ++;
        //if(a > 360) a = 0;
    }

    @Override
    public void onUpdate(SealEntity entity) {
        entity.rotation.set(a, 1, 1, 1);
        //if(a > 360) SealEngine.destroy(entity);
    }
}
