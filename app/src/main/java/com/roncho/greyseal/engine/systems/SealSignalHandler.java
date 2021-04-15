package com.roncho.greyseal.engine.systems;

import com.roncho.greyseal.engine.systems.stream.Entity;

public interface SealSignalHandler {
    void handleSignal(Entity e, Object... args);
}
