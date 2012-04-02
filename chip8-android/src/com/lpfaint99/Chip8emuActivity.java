package com.lpfaint99;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
public class Chip8emuActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        GLSurfaceView view = new GLSurfaceView(this);
        view.setRenderer(new Chip8());
        setContentView(view);
        
    }
}