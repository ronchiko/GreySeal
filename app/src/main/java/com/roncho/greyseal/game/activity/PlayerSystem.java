package com.roncho.greyseal.game.activity;

import com.roncho.greyseal.engine.Quaternion;
import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.SealMath;
import com.roncho.greyseal.engine.SealMetrics;
import com.roncho.greyseal.engine.Time;
import com.roncho.greyseal.engine.android.Input;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.engine.systems.stream.Entity;
import com.roncho.greyseal.game.GameAssets;

public class PlayerSystem extends SealSystem {

    public static final float minX = -2, maxX = 2;

    private static final float TIME_BETWEEN_SHOTS = 1;

    private float targetX = 0;

    private final float playerMoveSpeed = 10;
    private boolean touchingScreen;
    private float shootTimer = 0;
    private float toLoop = 0;

    @Override
    public boolean selectObject(Entity entity) {
        return (entity.userFlags & GameFlags.PLAYER.getMask()) != 0;
    }

    @Override
    public void updateOnce() {

        touchingScreen = false;
        // Read the touch inputs
        for(int i = 0; i < Input.touchCount(); i++){
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

        // If the ship is not looping and its moving enough for a loop, then trigger a loop
        if(toLoop <= 0 && SealMath.abs(entity.position.x - targetX) > 1f){
            toLoop = 360;
        }

        entity.position.x = SealMath.moveTowards(entity.position.x, targetX, Time.deltaTime() * playerMoveSpeed, 0.4f);

        if(toLoop > 0){
            // If the ship should rotate then modify its rotation and rotate it
            float rotAngle = 360 * Time.deltaTime();
            entity.rotation = entity.rotation.mul(Quaternion.euler(0, 0, rotAngle));
            toLoop -= rotAngle;
            // Check if the loop is done and reset rotation to prevent artifacts
            if(toLoop <= 0.1){
                toLoop = 0;
                entity.rotation = Quaternion.euler(0, 0, 0);
            }
        }

        // Create a laser if the screen was held for a long enough time
        if(touchingScreen && shootTimer >= TIME_BETWEEN_SHOTS){
            create(GameAssets.LASER_ENTITY, entity.position);
            shootTimer = 0;
        }
    }
}
