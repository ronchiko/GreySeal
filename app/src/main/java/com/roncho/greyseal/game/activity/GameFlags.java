package com.roncho.greyseal.game.activity;

public enum GameFlags {

    PLAYER(0x1),
    ENEMY(0x2),
    BULLET(0x4);

    private final int mask;

    GameFlags(int mask){ this.mask = mask; }

    public int getMask() {
        return mask;
    }
}
