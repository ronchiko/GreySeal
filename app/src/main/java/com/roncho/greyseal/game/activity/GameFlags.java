package com.roncho.greyseal.game.activity;

import com.roncho.greyseal.engine.api.MaskEnum;

public enum GameFlags implements MaskEnum {

    PLAYER(0x1),
    ENEMY(0x2),
    BULLET(0x4),
    GROUND(0x8),
    DECOR_PARALLAX(0x10);

    private final int mask;

    GameFlags(int mask){ this.mask = mask; }

    public int getMask() {
        return mask;
    }
}
