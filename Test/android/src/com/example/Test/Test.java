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
package com.example.Test;

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
import android.util.Log;


public class Test extends Activity
{
    public static Test m_test = null;
    private static String TAG = "Test";

    GL2JNIView mView;
    //SurfaceView mView;

    public static void kgmAppFinish()
    {
        System.out.println("Test Finishing");

        if(m_test != null)
        {
            TestLib.quit();
            m_test.finish();
        }
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

	m_test = this;
        Log.v(TAG, "Thread = " + Thread.currentThread().getId());
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
    		GL2JNIView.isEvt = true;
    		GL2JNIView.evtId = GL2JNIView.EVT_TOUCH;
    		GL2JNIView.evtKey = 0;
    		GL2JNIView.evtX = (int)me.getX();
    		GL2JNIView.evtY = (int)me.getY();
        	//TestLib.onTouch(0, (int)me.getX(), (int)me.getY());
    		break;
    	case MotionEvent.ACTION_DOWN:
    		GL2JNIView.isEvt = true;
    		GL2JNIView.evtId = GL2JNIView.EVT_TOUCH;
    		GL2JNIView.evtKey = 1;
    		GL2JNIView.evtX = (int)me.getX();
    		GL2JNIView.evtY = (int)me.getY();
        	//TestLib.onTouch(1, (int)me.getX(), (int)me.getY());
    		break;
    	case MotionEvent.ACTION_UP:
    		GL2JNIView.isEvt = true;
    		GL2JNIView.evtId = GL2JNIView.EVT_TOUCH;
    		GL2JNIView.evtKey = 2;
    		GL2JNIView.evtX = (int)me.getX();
    		GL2JNIView.evtY = (int)me.getY();
        	//TestLib.onTouch(2, (int)me.getX(), (int)me.getY());
    		break;
    	}
    	//TestLib.onMsMove((int)me.getX(), (int)me.getY());
    	//TestLib.onMsMove();

    	return true;
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
		GL2JNIView.isEvt = true;
		GL2JNIView.evtId = GL2JNIView.EVT_KEYDOWN;
		GL2JNIView.evtKey = keyCode;
        //TestLib.onKeyboard(0, keyCode);

    	return true;
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
		GL2JNIView.isEvt = true;
		GL2JNIView.evtId = GL2JNIView.EVT_KEYUP;
		GL2JNIView.evtKey = keyCode;
        //TestLib.onKeyboard(1, keyCode);

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
