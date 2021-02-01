package com.roncho.greyseal;

import com.roncho.greyseal.engine.SealEngineActivity;
import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.engine.systems.test.RotateSystem;

public class MainActivity extends SealEngineActivity {

    @Override
    public void loadResources() {
        SealSystemManager.addSystem(new RotateSystem());
    }
}