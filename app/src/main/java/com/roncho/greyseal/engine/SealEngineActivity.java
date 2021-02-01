package com.roncho.greyseal.engine;

import android.content.res.AssetManager;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

import com.roncho.greyseal.engine.android.SealSurfaceView;
import com.roncho.greyseal.engine.android.SealTexturePipeline;
import com.roncho.greyseal.engine.systems.SealSystemManager;

public abstract class SealEngineActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("greyseal-core");
    }

    private SealSurfaceView surface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        surface = new SealSurfaceView(getBaseContext());
        startEngine(getAssets());
        SealTexturePipeline.init(getAssets());
        SealSystemManager.newManager();

        loadResources();

        setContentView(surface);
    }

    @Override
    protected void onPause() {
        super.onPause();
        surface.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        surface.onResume();
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        stopEngine();
    }

    public abstract void loadResources();

    private native static void startEngine(AssetManager manager);
    private native static void stopEngine();
}
