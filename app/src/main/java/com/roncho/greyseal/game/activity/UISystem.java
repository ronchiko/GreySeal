package com.roncho.greyseal.game.activity;

import android.content.SharedPreferences;

import com.roncho.greyseal.engine.Rect;
import com.roncho.greyseal.engine.SealEngine;
import com.roncho.greyseal.engine.SealEngineActivity;
import com.roncho.greyseal.engine.SealMath;
import com.roncho.greyseal.engine.SealMetrics;
import com.roncho.greyseal.engine.UI;
import com.roncho.greyseal.engine.android.cpp.SealLinkedCache;
import com.roncho.greyseal.engine.systems.SealStaticSystem;
import com.roncho.greyseal.game.GameAssets;

public class UISystem extends SealStaticSystem {

    private SharedPreferences preferences = SealEngineActivity.getPreferences("highscore");

    private int score, wave;

    private static class MenuUI {

        private float m_Y, m_Target = 255;
        private float m_Alpha = 255;

        private UI.UIObject title;
        private UI.UIObject highScoreDisplay;
        private UI.UIObject instructionsText;

        private UI.UIObject[] m_Children;
        private float[] m_Offsets;

        public MenuUI(){
            int font = SealEngine.loadFont(GameAssets.FONT_IMPACT);
            int sWidth = SealMetrics.getScreenWidth();
            int sHeight = SealMetrics.getScreenHeight();

            title = UI.newUiObject()
                    .setBackgroundImage(SealLinkedCache.getTexture("textures/title.png"))
                    .setTransform(sWidth * 0.125f, -500, sWidth * 0.75f, sWidth * 0.35f);
            highScoreDisplay = UI.newUiObject().makeTransparent().
                    setFont(font).setText("High Score: " + instance.preferences.getInt("High Score", 0))
                    .setTextColor(0, 0, 0, 255).
                    setTransform(0, -500, sWidth, 50).
                    setPadding(2, 2, 2, 2).setTextSize(32).
                    setTextAlignment(UI.Alignment.Center, UI.Alignment.Center);;
            instructionsText = UI.newUiObject().makeTransparent()
                    .setFont(font).setText("Move to start game").setTextColor(0, 0, 0, 255).
                    setTransform(0, -500, sWidth, 50).
                    setPadding(2, 2, 2, 2).setTextSize(32).
                    setTextAlignment(UI.Alignment.Center, UI.Alignment.Center);

            m_Y = -sHeight;

            m_Children = (UI.UIObject[]) new UI.UIObject[]{title, highScoreDisplay, instructionsText};
            m_Offsets  = (float[]) new float[]{100, 0, 50};
        }

        public void update(){
            m_Y = SealMath.lerp(m_Y, 0, 0.1f);
            m_Alpha = SealMath.lerp(m_Alpha, m_Target, 0.1f);
            int i = 0;
            for(UI.UIObject child : m_Children){
                Rect rect = child.getTransform();
                child.setAlpha((int)m_Alpha);
                child.setTransform(rect.x, m_Y + m_Offsets[i], rect.w, rect.h);
                i++;
            }
        }

        public void close(){
            m_Target = 0;
        }

        public boolean closed(){
            return m_Target == 0 && SealMath.abs(m_Alpha - m_Target) < 0.1f;
        }

        public void delete(){
            for(UI.UIObject child : m_Children) child.delete();
        }
    }

    private class GameUI {
        private UI.UIObject scoreText, waveText;

        public GameUI() {
            int fontImpact = SealEngine.loadFont(GameAssets.FONT_IMPACT);
            int sWidth = SealMetrics.getScreenWidth();
            scoreText = UI.newUiObject();
            scoreText.setFont(fontImpact).setTextSize(24).
                    setPadding(5, 5, 5, 5).
                    setTransform(0, 0, sWidth, 30).makeTransparent()
                    .setTextColor(0, 0, 0, 255).setTextAlignment(UI.Alignment.Left, UI.Alignment.Center);
            waveText = UI.newUiObject();
            waveText.setFont(fontImpact).setTextSize(24).setPadding(5, 5, 5, 5).
                    setTransform(0, 40, sWidth, 30).makeTransparent()
                    .setTextAlignment(UI.Alignment.Left, UI.Alignment.Center).setTextColor(0, 0, 0, 255);
        }

        public void update(){
            scoreText.setText("Score: " + score);
            waveText.setText("Wave: " + wave);
        }

    }

    private static UISystem instance;
    private MenuUI menu;
    private GameUI gameUI;

    public UISystem(){
        instance = this;
        wave = 1;
        score = 0;
    }

    @Override
    public void updateOnce() {
        if(gameUI == null){
            gameUI = new GameUI();
            resetScore();
        }

        if(menu != null) {
            menu.update();

            if(menu.closed()) {
                menu.delete();
            }
        }

        gameUI.update();
    }

    public static void addScore(int score){
        if(instance != null){
            instance.score += score;
        }
    }
    public static void resetScore(){
        if(instance != null){
            // Read from the high score and check if it was broken, if so - update it.
            int highScore = instance.preferences.getInt("High Score", 0);
            if(instance.score > highScore) {
                SharedPreferences.Editor highScoreEditor = instance.preferences.edit();
                highScoreEditor.putInt("High Score", instance.score);
                highScoreEditor.apply();
            }

            instance.score = 0;
            instance.wave = 1;
            instance.menu = new MenuUI();
        }
    }
    public static void incrementWave(){
        if(instance != null){
            instance.wave++;
        }
    }
    public static void closeMenu(){
        if(instance != null){
            if(instance.menu != null) instance.menu.close();
        }
    }
}
