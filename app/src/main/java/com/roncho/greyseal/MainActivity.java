package com.roncho.greyseal;

import android.util.DisplayMetrics;

import com.roncho.greyseal.engine.SealEngine;
import com.roncho.greyseal.engine.SealEngineActivity;
import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.engine.systems.test.MoveSystem;
import com.roncho.greyseal.engine.systems.test.RotateSystem;

public class MainActivity extends SealEngineActivity {

    @Override
    public void loadResources() {
        SealSystemManager.addSystem(new RotateSystem());
        SealSystemManager.addSystem(new MoveSystem());

        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        MoveSystem.height = displayMetrics.heightPixels;
        MoveSystem.width = displayMetrics.widthPixels;

        SealEngine.loadTexture("textures/seal.jpg");
    }
}