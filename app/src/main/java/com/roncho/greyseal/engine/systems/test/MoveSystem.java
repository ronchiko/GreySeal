package com.roncho.greyseal.engine.systems.test;

import android.util.DisplayMetrics;
import android.util.Log;

import com.roncho.greyseal.engine.TouchInput;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.engine.systems.stream.SealEntity;

public class MoveSystem extends SealSystem {

    public static int width, height;

    @Override
    public boolean selectObject(SealEntity entity) {
        return (entity.userFlags & 0x2) != 0;
    }

    @Override
    public void onUpdate(SealEntity entity) {
        
    }
}
