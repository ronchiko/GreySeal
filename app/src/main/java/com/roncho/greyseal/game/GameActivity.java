package com.roncho.greyseal.game;

import com.roncho.greyseal.engine.Quaternion;
import com.roncho.greyseal.engine.SealEngine;
import com.roncho.greyseal.engine.SealEngineActivity;
import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.physics.CollisionSystem;
import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.game.activity.EnemySystem;
import com.roncho.greyseal.game.activity.GameManagerSystem;
import com.roncho.greyseal.game.activity.ParallaxSystem;
import com.roncho.greyseal.game.activity.PlayerSystem;
import com.roncho.greyseal.game.activity.ProjectileSystem;
import com.roncho.greyseal.game.activity.UISystem;

public class GameActivity extends SealEngineActivity {
    @Override
    public void loadResources(InitialCameraState state) {
        SealEngine.loadMesh("models/spaceship.obj");
        SealEngine.loadMesh("models/ufo.obj");
        SealEngine.loadMesh("models/laser.obj");

        SealEngine.loadTexture("textures/spaceship-uvl.png");
        SealEngine.loadTexture("textures/ufo-uvl.png");
        SealEngine.loadTexture("textures/title.png");

        SealEngine.loadMaterial("vertex.glsl", "fragment.glsl");

        SealEngine.makePreset("presets/player.ntt", new Vector3(0,  -3, -3));

        state.rotation(Quaternion.euler(-30, 0, 0));
        state.position(0, 1, 0);

        SealSystemManager.addSystem(new GameManagerSystem());
        SealSystemManager.addSystem(new PlayerSystem());
        SealSystemManager.addSystem(new EnemySystem());
        SealSystemManager.addSystem(new ProjectileSystem());
        SealSystemManager.addSystem(new UISystem());
        SealSystemManager.addSystem(new ParallaxSystem());

        CollisionSystem.enable();
    }
}
