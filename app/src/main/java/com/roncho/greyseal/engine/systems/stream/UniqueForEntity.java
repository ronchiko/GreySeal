package com.roncho.greyseal.engine.systems.stream;

import java.util.HashMap;

public class UniqueForEntity<T> {

    private final HashMap<Short, T> m_Map;

    public UniqueForEntity() {
        m_Map = new HashMap<>();
    }

    public final T add(Entity e, T instance){
        m_Map.put(e.uid, instance);
        return instance;
    }

    public final T get(Entity e){
        return m_Map.get(e.uid);
    }

    public final void remove(Entity e){
        m_Map.remove(e.uid);
    }
}
