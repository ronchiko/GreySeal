package com.roncho.greyseal.game.activity;

import com.roncho.greyseal.engine.SealEngine;
import com.roncho.greyseal.engine.SealMath;
import com.roncho.greyseal.engine.SealMetrics;
import com.roncho.greyseal.engine.Time;
import com.roncho.greyseal.engine.UI;
import com.roncho.greyseal.engine.android.Input;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.engine.systems.stream.Entity;

public class PlayerSystem extends SealSystem {

    public static final float minX = -2, maxX = 2;

    private UI.UIObject testUi;
    private float targetX = 0;

    private final float playerMoveSpeed = 10;
    private boolean touchingScreen;
    private float shootTimer = 0;

    @Override
    public boolean selectObject(Entity entity) {
        return (entity.userFlags & GameFlags.PLAYER.getMask()) != 0;
    }

    @Override
    public void updateOnce() {

        if(testUi == null) {
            SealEngine.loadFont("fonts/ADDECRG.TTF");
            testUi = UI.newUiObject();
            testUi.setTransform(50, 50, 300, 300);
            testUi.setText("Hello Motherfucker world!");
        }

        touchingScreen = false;
        for(int i = 0; i < Input.touchCount(); i++ ){
            if(SealMetrics.percentageY(Input.touch(i).y()) > 0.7f){
                if(!GameManagerSystem.inGame()) GameManagerSystem.startGame();
                targetX = (1 - SealMetrics.percentageX(Input.touch(i).x())) * (maxX - minX) + minX;
                touchingScreen = true;
                break;
            }
        }

        if(!touchingScreen) shootTimer = 0;
        else shootTimer += Time.deltaTime();
    }

    @Override
    public void onUpdate(Entity entity) {
        entity.position.x = SealMath.moveTowards(entity.position.x, targetX, Time.deltaTime() * playerMoveSpeed, 0.4f);

        if(touchingScreen){
            // Then shoot
        }
    }
}
