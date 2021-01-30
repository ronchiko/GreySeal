package com.roncho.greyseal.engine;

import android.content.res.AssetManager;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

import com.roncho.greyseal.engine.android.GreySealSurfaceView;
import com.roncho.greyseal.engine.android.GreySealTexturePipeline;

public class GreySealEngineActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("greyseal-core");
    }

    private GreySealSurfaceView surface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        surface = new GreySealSurfaceView(getBaseContext());
        startEngine(getAssets());
        GreySealTexturePipeline.init(getAssets());

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

    private native static void startEngine(AssetManager manager);
    private native static void stopEngine();
}
