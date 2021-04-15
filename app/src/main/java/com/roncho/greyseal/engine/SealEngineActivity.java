package com.roncho.greyseal.engine;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.DragEvent;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.roncho.greyseal.engine.android.SealSurfaceView;
import com.roncho.greyseal.engine.android.SealTexturePipeline;
import com.roncho.greyseal.engine.systems.SealSystemManager;

public abstract class SealEngineActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("greyseal-core");
    }

    protected static class InitialCameraState {
        private final float[] transform;

        private InitialCameraState(){
            transform = new float[7];
            rotation(Quaternion.euler(0, 0, 0));
            position(0, 0, 0);
        }

        public void rotationEuler(float x, float y, float z){
            Quaternion q = Quaternion.euler(x, y, z);
            rotation(q);
        }

        public void rotation(float w, float x, float y, float z){
            transform[3] = w;
            transform[4] = x;
            transform[5] = y;
            transform[6] = z;
        }

        public void rotation(Quaternion q){
            transform[3] = q.w;
            transform[4] = q.x;
            transform[5] = q.y;
            transform[6] = q.z;
        }

        public void position(float x, float y, float z){
            transform[0] = x;
            transform[1] = y;
            transform[2] = z;
        }
    }

    private SealSurfaceView surface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        surface = new SealSurfaceView(getBaseContext());
        startEngine(getAssets());
        SealTexturePipeline.init(getAssets());
        SealSystemManager.newManager();
        SealMetrics.reload(this);
        InitialCameraState cameraState = new InitialCameraState();
        loadResources(cameraState);
        preloadCamera(cameraState.transform);
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

    public abstract void loadResources(InitialCameraState state);

    private native static void startEngine(AssetManager manager);
    private native static void stopEngine();
    private native static void preloadCamera(float[] camera);
}
