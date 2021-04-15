package com.roncho.greyseal.game.activity;

import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.SealMath;
import com.roncho.greyseal.engine.Time;
import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.systems.SealStaticSystem;

public class GameManagerSystem extends SealStaticSystem {

    private static final float m_Range = 4;
    private static GameManagerSystem instance;

    private final static class WaveManagerSystem {

        private static final int m_FinalRound = 10; // After this round shit about be tough
        private static final float m_HalfRange = m_Range * 0.5f;

        private WaveManagerSystem(){
            m_CurrentWave = 0;
            m_Wave = null;
            timer = 4;
            newWave();
        }

        private static class AttackFormation {
            private class Attacker {
                private int m_AttackerType;
                private float m_Offset;

                public Attacker(int type, float off){
                    m_Offset = off;
                    m_AttackerType = type;
                }
            }

            private final float m_ZSummon = -50;
            private final Attacker[] m_Attacker;

            public AttackFormation(int round){
                int attackers = SealMath.clamp(round / (m_FinalRound), 1, 5);       // Select the amount of attackers

                float l_AttackersRelative = (attackers / 5f);
                m_Attacker = new Attacker[attackers];
                float l_Freedom =  - (l_AttackersRelative * l_AttackersRelative + 1) * m_HalfRange;
                for(int i = 0; i < attackers; i++){
                    float l_Position = (PlayerSystem.maxX * i +  PlayerSystem.minX * (5 - i)) / 5f + SealMath.random(-l_Freedom, l_Freedom);
                    m_Attacker[i] = new Attacker(0,  l_Position);
                }
            }

            public void spawn(){
                for (Attacker attacker :
                        m_Attacker) {
                    instantiate("presets/ufo.ntt", new Vector3(attacker.m_Offset, -3, m_ZSummon));
                }
            }
        }

        private static class Wave {
            private float m_EnemySpeedModifier = 1;
            private float m_EnemyHealthModifier = 1;

            private final AttackFormation[] m_Formations;
            private int m_Current;

            public Wave(int round){
                m_Current = 0;
                int l_FormationCount = SealMath.isqrt(round) + 3;
                m_Formations = new AttackFormation[l_FormationCount];
                for(int i = 0; i < l_FormationCount; i++){
                    m_Formations[i] = new AttackFormation(round);
                }
            }

            public boolean hasNext() { return m_Current < m_Formations.length; }
            public AttackFormation next() { return m_Formations[m_Current++]; }
        }

        private int m_CurrentWave;
        private float timer = 4;
        private Wave m_Wave;

        public void tick(){
            timer += Time.deltaTime();
            if(timer >= 5 && m_Wave != null){
                if(m_Wave.hasNext()){
                    m_Wave.next().spawn();
                }
                timer = 0;
            }
        }

        public void newWave() {
            m_Wave = new Wave(++m_CurrentWave);
        }
    }

    public enum _GameState {
        IDLE(0),
        PLAYING(1),
        END(2);

        private int uid;

        _GameState(int uid) { this.uid = uid; }
    }

    private static final float _summonZ = -50;
    private _GameState state;
    private WaveManagerSystem manager;

    public GameManagerSystem(){
        state = _GameState.IDLE;
        instance = this;
    }

    @Override
    public void updateOnce() {
        switch (state){
            case PLAYING:
                if(manager != null){
                    manager.tick();
                }else manager = new WaveManagerSystem();
                break;
            case IDLE:
            case END:
                break;
        }
    }

    public static boolean inGame() { return instance.state == _GameState.PLAYING; }

    public static void startGame(){
        if(instance.state != _GameState.PLAYING)
            instance.state = _GameState.PLAYING;
    }
}
