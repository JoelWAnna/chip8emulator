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
        final Chip8Renderer chip8 = new Chip8Renderer();
        view.setRenderer(chip8);
        setContentView(view);
        Thread t = new Thread( new Runnable() {

			public void run() {
				chip8.foo = new Chip8core();
    	byte[] buffer = {
			(byte)0xA2, (byte)0xCC, (byte)0x6A, (byte)0x06, (byte)0x61, (byte)0x03,
			(byte)0x6B, (byte)0x08, (byte)0x60, (byte)0x00, (byte)0xD0, (byte)0x11,
			(byte)0x70, (byte)0x08, (byte)0x7B, (byte)0xFF, (byte)0x3B, (byte)0x00,
			(byte)0x12, (byte)0x0A, (byte)0x71, (byte)0x02, (byte)0x7A, (byte)0xFF,
			(byte)0x3A, (byte)0x00, (byte)0x12, (byte)0x06, (byte)0x66, (byte)0x00,
			(byte)0x67, (byte)0x14, (byte)0xA2, (byte)0xCD, (byte)0x60, (byte)0x20,
			(byte)0x61, (byte)0x1E, (byte)0xD0, (byte)0x11, (byte)0x63, (byte)0x1D,
			(byte)0x62, (byte)0x3F, (byte)0x82, (byte)0x02, (byte)0x77, (byte)0xFF,
			(byte)0x47, (byte)0x00, (byte)0x12, (byte)0xAA, (byte)0xFF, (byte)0x0A,
			(byte)0xA2, (byte)0xCB, (byte)0xD2, (byte)0x31, (byte)0x65, (byte)0xFF,
			(byte)0xC4, (byte)0x01, (byte)0x34, (byte)0x01, (byte)0x64, (byte)0xFF,
			(byte)0xA2, (byte)0xCD, (byte)0x6C, (byte)0x00, (byte)0x6E, (byte)0x04,
			(byte)0xEE, (byte)0xA1, (byte)0x6C, (byte)0xFE, (byte)0x6E, (byte)0x06,
			(byte)0xEE, (byte)0xA1, (byte)0x6C, (byte)0x02, (byte)0xD0, (byte)0x11,
			(byte)0x80, (byte)0xC4, (byte)0xD0, (byte)0x11, (byte)0x4F, (byte)0x01,
			(byte)0x12, (byte)0x98, (byte)0x42, (byte)0x00, (byte)0x64, (byte)0x01,
			(byte)0x42, (byte)0x3F, (byte)0x64, (byte)0xFF, (byte)0x43, (byte)0x00,
			(byte)0x12, (byte)0xCE, (byte)0x43, (byte)0x1F, (byte)0x12, (byte)0xA4,
			(byte)0xA2, (byte)0xCB, (byte)0xD2, (byte)0x31, (byte)0x82, (byte)0x44,
			(byte)0x83, (byte)0x54, (byte)0xD2, (byte)0x31, (byte)0x3F, (byte)0x01,
			(byte)0x12, (byte)0x42, (byte)0x43, (byte)0x1E, (byte)0x12, (byte)0x98,
			(byte)0x6A, (byte)0x02, (byte)0xFA, (byte)0x18, (byte)0x76, (byte)0x01,
			(byte)0xA2, (byte)0xCA, (byte)0x12, (byte)0x88, (byte)0xD2, (byte)0x31,
			(byte)0xC4, (byte)0x01, (byte)0x34, (byte)0x01, (byte)0x64, (byte)0xFF,
			(byte)0xC5, (byte)0x01, (byte)0x35, (byte)0x01, (byte)0x65, (byte)0x01,
			(byte)0x12, (byte)0x42, (byte)0x6A, (byte)0x03, (byte)0xFA, (byte)0x18,
			(byte)0xA2, (byte)0xCB, (byte)0xD2, (byte)0x31, (byte)0x73, (byte)0xFF,
			(byte)0x12, (byte)0x36, (byte)0xA2, (byte)0xCB, (byte)0xD2, (byte)0x31,
			(byte)0x12, (byte)0x28, (byte)0xA2, (byte)0xCD, (byte)0xD0, (byte)0x11,
			(byte)0xA2, (byte)0xF0, (byte)0xF6, (byte)0x33, (byte)0xF2, (byte)0x65,
			(byte)0x63, (byte)0x18, (byte)0x64, (byte)0x1B, (byte)0xF0, (byte)0x29,
			(byte)0xD3, (byte)0x45, (byte)0x73, (byte)0x05, (byte)0xF1, (byte)0x29,
			(byte)0xD3, (byte)0x45, (byte)0x73, (byte)0x05, (byte)0xF2, (byte)0x29,
			(byte)0xD3, (byte)0x45, (byte)0x12, (byte)0xC8, (byte)0xF0, (byte)0x80,
			(byte)0xFF, (byte)0xFF, (byte)0xA2, (byte)0xDE, (byte)0x63, (byte)0x15,
			(byte)0x62, (byte)0x10, (byte)0xD3, (byte)0x25, (byte)0xA2, (byte)0xE3,
			(byte)0x63, (byte)0x1D, (byte)0xD3, (byte)0x25, (byte)0x12, (byte)0xAA,
			(byte)0xEE, (byte)0x8A, (byte)0xCE, (byte)0x8C, (byte)0x8A, (byte)0xEE,
			(byte)0x88, (byte)0xCC, (byte)0x88, (byte)0xEE
		};

    	chip8.foo.load_rom(buffer, buffer.length);}
		});
		t.run();
    }
}