package com.roncho.greyseal.game;

import com.roncho.greyseal.engine.api.HasStringValue;

public enum GameAssets implements HasStringValue {
    FONT_IMPACT("fonts/impact.ttf"),
    FONT_ADDECRG("fonts/ADDECRG.ttf"),

    LASER_ENTITY("presets/laser.ntt"),
    PLAYER_ENTITY("presets/player.ntt"),
    GROUND_ENTITY("presets/ground1.ntt"),
    UFO_ENTITY("presets/ufo.ntt"),

    DECOR_ROCK0("presets/decor/rock0.ntt"),
    DECOR_TREE0("presets/decor/tree0.ntt");

    String path;

    GameAssets(String path){
        this.path = path;
    }

    @Override
    public String getStringValue() {
        return path;
    }
}
