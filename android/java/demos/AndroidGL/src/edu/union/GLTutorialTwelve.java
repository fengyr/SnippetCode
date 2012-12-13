package edu.union;

import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLU;

/**
 * http://www.zeuscmd.com/tutorials/opengles/17-TextureMapping.php
 * @author bburns
 *
 */
public class GLTutorialTwelve extends GLTutorialBase {
	int tex;
	Bitmap bmp;
	
	Tunnel3D tunnel;
	
	public GLTutorialTwelve(Context c) {
		super(c, 20);
		bmp = BitmapFactory.decodeResource(c.getResources(), R.drawable.plants03);
		tunnel = new Tunnel3D(10, 20);
	}
	
	protected void init(GL10 gl) {
		gl.glEnable(GL10.GL_DEPTH_TEST);
		gl.glDepthFunc(GL10.GL_LEQUAL);
		
		gl.glEnable(GL10.GL_TEXTURE_2D);
		
		gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.glClearDepthf(1.0f);
		
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glEnableClientState(GL10.GL_COLOR_ARRAY);
		gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
		
		gl.glEnable(GL10.GL_CULL_FACE);
		gl.glShadeModel(GL10.GL_SMOOTH);
		
		float ratio = (float) width / height;
		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();
		gl.glViewport(0, 0, width, height);
		GLU.gluPerspective(gl, 45.0f, ratio, 1.0f, 100.0f);
		
		tex = loadTexture(gl, bmp);
	}
	
	protected void drawFrame(GL10 gl) {
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		
		// Setting up the modelview...
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();
		
		// Render the tunnel...    
		tunnel.render(gl, -1.2f);
		tunnel.nextFrame();
	}
}