/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.kgmEngine.Game;

import android.app.Activity;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.content.res.Configuration;


public class Game extends Activity
{
    public static Game m_game = null;

    //GL2JNIView mView;
    SurfaceView mView;

    public static void GameFinish()
    {
        System.out.println("Game Finishing");

        GameLib.quit();
        m_game.finish();
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
    	super.onCreate(savedInstanceState);

    	/* Create a TextView and set its content.
    	 * the text is retrieved by calling a native
    	 * function.
    	 */
    	// requesting to turn the title OFF
    	requestWindowFeature(Window.FEATURE_NO_TITLE);
    	// making it full screen
    	getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
    			WindowManager.LayoutParams.FLAG_FULLSCREEN);


    	mView = new GL2JNIView(getApplication(), true, 1, 0);
    	//mView = new TestView(getApplication());
    	//mView = new GLSurfaceView(this);
    	setContentView(mView);

	m_game = this;
    }
    
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
       //super.onConfigurationChanged(newConfig);

       // Checks the orientation of the screen
       if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
       } else if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT){
       }
    }

    @Override
    public boolean onTouchEvent(MotionEvent me)
    {
    	switch(me.getAction())
    	{
    	case MotionEvent.ACTION_MOVE:
        	GameLib.onTouch(0, (int)me.getX(), (int)me.getY());
    		break;
    	case MotionEvent.ACTION_DOWN:
        	GameLib.onTouch(1, (int)me.getX(), (int)me.getY());
    		break;
    	case MotionEvent.ACTION_UP:
        	GameLib.onTouch(2, (int)me.getX(), (int)me.getY());
    		break;
    	}
    	//TestLib.onMsMove((int)me.getX(), (int)me.getY());
    	//TestLib.onMsMove();
    	
    	return true;
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        GameLib.onKeyboard(0, keyCode);

    	return true;
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
        GameLib.onKeyboard(1, keyCode);

    	return true;
    }

    /* A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */
    /* This is another native method declaration that is *not*
     * implemented by 'hello-jni'. This is simply to show that
     * you can declare as many native methods in your Java code
     * as you want, their implementation is searched in the
     * currently loaded native libraries only the first time
     * you call them.
     *
     * Trying to call this function will result in a
     * java.lang.UnsatisfiedLinkError exception !
     */
}
