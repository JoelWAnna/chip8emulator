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
//http://blog.jayway.com/2009/12/03/opengl-es-tutorial-for-android-part-i/
public class Chip8 implements Renderer {

	private void drawrect(GL10 gl, Point p)
	{
		final int modifier = 2;
		
		float vertices[]  = {
				0,0,0,
				0,.5f,0,
				.5f,.5f,0,
				.5f,0,0,
				0,.5f,0,
				.5f,.5f,0,
				.5f,0,0,
				0,0,0,
		};
		float verticesa[] ={
				-1,-1,0,
				-1,1, 0,
				1,1,0,
				1,-1,0,
		//	(p.x * modifier), (p.y * modifier) + 0.0f,	 0.0f,
	//		(p.x * modifier) + 0.0f,     (p.y * modifier) + modifier, 0.0f,
	//		(p.x * modifier) + modifier, (p.y * modifier) + modifier, 0.0f,
	//		(p.x * modifier) + modifier, (p.y * modifier) + 0.0f,	 0.0f,
		};
		ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
		vbb.order(ByteOrder.nativeOrder());
		FloatBuffer vertexBuffer = vbb.asFloatBuffer();
		vertexBuffer.put(vertices);
		vertexBuffer.position(0);

		// Specifies the location and data format of an array of vertex
		// coordinates to use when rendering.
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);// OpenGL docs.
		//gl.glTranslatef(0, 0, -4);
			gl.glColor4f(1, 0, 1, .5f);
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer); // OpenGL docs.
		gl.glDrawArrays(GL10.GL_TRIANGLES, 0, 4 );
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);// OpenGL docs.
	}

	public void onDrawFrame(GL10 gl) {
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT | // OpenGL docs.
                GL10.GL_DEPTH_BUFFER_BIT);

	//When you are done with the buffer don't forget to disable it.
		drawrect(gl, new Point(0,0));
		
		///////////////////////
		// Disable the vertices buffer.
		//GL10.
		//gl.glVertexPointer(3, type, stride, pointer)
	}

	public void onSurfaceChanged(GL10 gl, int w, int h) {
		// Sets the current view port to the new size.
		gl.glViewport(0, 0, w, h);
		// Select the projection matrix
		gl.glMatrixMode(GL10.GL_PROJECTION);
		// Reset the projection matrix
		gl.glLoadIdentity();// OpenGL docs.
		//gl.glOrthof(0, w, h, 0, 0, 1);
		
		// Calculate the aspect ratio of the window
	/*	GLU.gluPerspective(gl, 45.0f,
                                   (float) w / (float) h,
                                   0.1f, 100.0f);
	*/	// Select the modelview matrix
		gl.glMatrixMode(GL10.GL_MODELVIEW);// OpenGL docs.
		// Reset the modelview matrix
	//	gl.glLoadIdentity();// OpenGL docs.
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig arg1) {
		gl.glClearColor(0, 2, 0, 0.5f);
		gl.glColor4f(1, 1, 1, .5f);
	}

}
