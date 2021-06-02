package com.roncho.greyseal.game.activity;

import com.roncho.greyseal.engine.Quaternion;
import com.roncho.greyseal.engine.SealMath;
import com.roncho.greyseal.engine.Time;
import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.android.cpp.SealLinkedCache;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.engine.systems.stream.Entity;
import com.roncho.greyseal.game.GameAssets;

public class ParallaxSystem extends SealSystem {
    private static final float horizon = -120;
    private static final float finishLane = 4;
    private static final float initialY = -15, desiredY = -5;

    private int queued = 16;

    private static float getObjectPositionOffset(int mesh, float x){
        //SealLog.Log("Mesh: " + mesh + " Rock: " + SealLinkedCache.getMesh("models/rock0.obj") + " Tree: " + SealLinkedCache.getMesh("models/tree.obj"));
        if(mesh == SealLinkedCache.getMesh("models/rock1.obj")){
            return  SealMath.abs(x) > 6 ? 2.5f : 0;
        }else if(mesh == SealLinkedCache.getMesh("models/tree.obj")){
            return SealMath.abs(x) < 4 ? 0 : 2.5f;
        }

        return 0;
    }

    @Override
    public boolean selectObject(Entity entity) {
        return entity.matches(GameFlags.GROUND) ||
                entity.matches(GameFlags.DECOR_PARALLAX);
    }

    private static void createRock(float planeZ, float oy){
        final float range = 30;
        float x = SealMath.random(-0.5f * range, 0.5f * range);
        float z = SealMath.random(-0.5f * range, 0.5f * range);
        float yRot = SealMath.random(0, 360);
        float xTilt = SealMath.random(-50, 50), zTilt = SealMath.random(-50, 50);

        float y = getObjectPositionOffset(SealLinkedCache.getMesh("models/rock0.obj"), x);

        float xScale = SealMath.random(0.4f, 1.1f), yScale = SealMath.random(0.4f, 1.1f), zScale = SealMath.random(0.4f, 1.1f);

        create(GameAssets.DECOR_ROCK0, new Vector3(x, oy + y, z + planeZ),
                Quaternion.euler(xTilt, yRot, zTilt), new Vector3(xScale, yScale, zScale));
    }

    private static void createTree(float planeZ, float oy){
        final float range = 30;
        float x = SealMath.random(-0.5f * range, 0.5f * range);

        if(x > 3 && x < 8) x += 5;
        if(x < -3 && x > -8) x -= 5;

        float z = SealMath.random(-0.5f * range, 0.5f * range);
        float yRot = SealMath.random(0, 360);
        float xTilt = SealMath.random(-10, 10), zTilt = SealMath.random(-10, 10);

        float scale = SealMath.random(0.15f, 0.2f);

        float y = getObjectPositionOffset(SealLinkedCache.getMesh("models/tree.obj"), x);

        create(GameAssets.DECOR_TREE0, new Vector3(x, oy + y, z + planeZ),
                Quaternion.euler(xTilt, yRot, zTilt), new Vector3(scale, scale, scale));
    }

    private static void createPlane(float planeZ, float y){
        create(GameAssets.GROUND_ENTITY, new Vector3(0, y, planeZ));

        int decors = SealMath.randomInt(0, 12);
        for(int i = 0; i < decors; i++){
            if(SealMath.randomInt(0, 2) == 1) createTree(planeZ, y);
            else createRock(planeZ, y);
        }
    }

    @Override
    public void updateOnce() {
        if(queued > 0){
            // If we start all planes at the initial height it gives an illusion of landing
            float position = horizon;
            for(int i = 0; i < queued; i++){
                createPlane(position, initialY);
                position += 8;
            }

            queued = 0;
        }
    }

    @Override
    public void onUpdate(Entity entity) {
        entity.position.z += 5 * Time.deltaTime();
        entity.position.y = SealMath.lerp(entity.position.y,
                desiredY + getObjectPositionOffset(entity.mesh, entity.position.x), 0.3f * Time.deltaTime());
        if(entity.position.z >= finishLane){
            destroy(entity);
            if(entity.matches(GameFlags.GROUND)) {
                createPlane(horizon - (finishLane - entity.position.z), initialY);
            }
        }
    }
}
