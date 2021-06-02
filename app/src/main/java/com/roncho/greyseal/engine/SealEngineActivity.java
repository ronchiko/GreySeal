package com.roncho.greyseal.engine;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.graphics.Path;
import android.os.Bundle;
import android.view.DragEvent;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.roncho.greyseal.engine.android.SealSurfaceView;
import com.roncho.greyseal.engine.android.SealTexturePipeline;
import com.roncho.greyseal.engine.api.HasStringValue;
import com.roncho.greyseal.engine.api.Utilities;
import com.roncho.greyseal.engine.systems.SealSystem;
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
    private static SealEngineActivity current;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        current = this;
        // Create the surface view and initialize the engine
        surface = new SealSurfaceView(getBaseContext());
        startEngine(getAssets());
        SealTexturePipeline.init(getAssets());
        SealSystemManager.newManager();
        SealMetrics.reload(this);
        // Prepare the base camera state and call the user's custom loaders
        InitialCameraState cameraState = new InitialCameraState();
        SealEngine.loadTexture("textures/button.png");
        loadResources(cameraState);
        preloadCamera(cameraState.transform);
        // Apply the surface view
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
        SealLog.Log("Finalizing");
        stopEngine();
    }

    public abstract void loadResources(InitialCameraState state);

    private native static void startEngine(AssetManager manager);
    private native static void stopEngine();
    private native static void preloadCamera(float[] camera);

    public void register(SealSystem system) { SealSystemManager.addSystem(system); }

    /**
     * Loads a resource, returns the fonts identifier if a font was loaded.
     * otherwise returns 0 if the load was successful, otherwise return -1
     * @param path
     * @return
     */
    public static int loadResource(HasStringValue path){
        return loadResource(path.getStringValue());
    }
    public static int loadResource(String path){
        switch (Utilities.getAssetType(path)){
            case FONT_FILE: return SealEngine.loadFont(path);
            case IMAGE_FILE: SealEngine.loadTexture(path); break;
            case MODEL_FILE: SealEngine.loadMesh(path); break;

            case ENTITY_FILE:
            case SHADER_FILE:
            case UNKNOWN:
                return -1;
        }
        return 0;
    }

    public static SharedPreferences getPreferences(String name){
        Context context = current;
        return context.getSharedPreferences(name, MODE_PRIVATE);
    }
}
