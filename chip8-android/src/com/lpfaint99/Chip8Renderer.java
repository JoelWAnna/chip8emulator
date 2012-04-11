package com.lpfaint99;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.graphics.Point;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLU;
import android.util.Log;
//http://blog.jayway.com/2009/12/03/opengl-es-tutorial-for-android-part-i/
public class Chip8Renderer implements Renderer {

	private int modifier;
	Chip8core foo;
    private long timePreviousNS = System.nanoTime();
	public Chip8Renderer() {

		foo = new Chip8core();
	}

	private FloatBuffer[] rectbuffer(Point origin, float size)
	{
	//	0..64
	//	
//	/	-1..1
		
		float xorigin = (origin.x - 32) / 32;
		float yorigin = (origin.y - 16) / 16 * -1;
		float xend = (origin.x + size -32) / 32;
		float yend = (origin.y +size - 16) / 16 * -1;

		float t1_verts[] = {
				xorigin, yorigin, 0,
				xorigin, yend, 0,
				xend, yend, 0,
		};
		float t2_verts [] = {

				xorigin, yorigin, 0,
				xend, yend, 0,
				xend, yorigin, 0,
		};
		FloatBuffer f[] = new FloatBuffer[2];
		f[0] = asfloatBuffer(t1_verts);
		f[1] = asfloatBuffer(t2_verts);
		return f;

	}

	private FloatBuffer asfloatBuffer(float [] vertices)
	{
		ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
		vbb.order(ByteOrder.nativeOrder());
		FloatBuffer vertexBuffer = vbb.asFloatBuffer();
		vertexBuffer.put(vertices);
		vertexBuffer.position(0);
		return vertexBuffer;
	}

	private void drawRECT(GL10 gl, Point origin)
	{
		//modifier = 10;
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);// OpenGL docs.
		FloatBuffer f[] = rectbuffer(origin, modifier);
		for (FloatBuffer x : f)
		{
		//gl.glTranslatef(0, 0, -4);
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, x); // OpenGL docs.
		gl.glDrawArrays(GL10.GL_TRIANGLES, 0, 4 );

		}
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);// OpenGL docs.
	}

	public void onDrawFrame(GL10 gl) {
		if (foo != null && foo.isloaded) {
		foo.runCycle();
		if (foo.isdirty())
		{
			gl.glClear(GL10.GL_COLOR_BUFFER_BIT | // OpenGL docs.
	                GL10.GL_DEPTH_BUFFER_BIT);
			renderSprites(gl);
		}
		if (foo.needsBeep())
			;
		}
		
        // Update base time values.
        final long  timeCurrentNS = System.nanoTime();
        final long  timeDeltaNS = timeCurrentNS - timePreviousNS;
        timePreviousNS = timeCurrentNS;
         
        // Determine time since last frame in seconds.
        final float timeDeltaS = timeDeltaNS * 1.0e-9f;
         
        // Print a notice if rendering falls behind 60Hz.
        if( timeDeltaS > (1.0f / 60.0f) )
        {
            Log.d( "SpikeTest", "Spike: " + timeDeltaS );
        }
		//drawRECT(gl, new Point(0,0));
		
	}

	private void renderSprites(GL10 gl) {
		for (int x = 0; x < 64; ++x)
		{
			for (int y = 0; y < 32; ++y)
			{
				if (foo.gfx[x+(y*64)] != 0)
				{
					if (x%2 > 0)
						gl.glColor4f(0, 1, 0, .5f);
					else if (y%2 > 0)
						gl.glColor4f(0, 0, 1, .5f);
					else
						gl.glColor4f(1, 0, 0, .5f);
				}
				else
					gl.glColor4f(0, 0, 0, .5f);

				drawRECT(gl, new Point(x,y));
			}
		}
		
	}

	public void onSurfaceChanged(GL10 gl, int w, int h) {
		gl.glViewport(0, 0, w, h);
		
		float ratio = (float)w / (float) h;
		
		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();
		//gl.glFrustumf(-ratio, ratio, -1, 1, 3, 7);  // apply the projection matrix
		  
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();
		//gl.glOrthof(0.0f, (float)w, (float) h, 0.0f, 0.0f,1.0f);
		
		//  GLU.gluLookAt(gl, 0, 0, -5, 0f, 0f, 0f, 0f, 1.0f, 0.0f);
		     // Calculate the aspect ratio of the window
		GLU.gluPerspective(gl, 45.0f, (float) w / (float) h,  -1, 1);
		modifier = w/64;
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig arg1) {
		gl.glClearColor(0, 2, 0, 0.5f);
		gl.glColor4f(1, 1, 1, .5f);
	}

}
