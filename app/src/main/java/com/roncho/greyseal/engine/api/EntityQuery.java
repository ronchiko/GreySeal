package com.roncho.greyseal.engine.api;

import com.roncho.greyseal.engine.systems.stream.Entity;

public interface EntityQuery {
    boolean query(Entity e);
}
