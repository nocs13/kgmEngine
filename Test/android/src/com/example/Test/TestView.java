package com.example.Test;

import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.graphics.Rect;
import android.os.SystemClock;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class TestView extends SurfaceView
{
	public class IdleTask extends TimerTask{
		@Override
		public void run(){
			//Log.v(TAG, "idle");
			TestLib.idle();
		}
	}
	
	private static String TAG = "TestView";
	public static Surface surface = null;
    public static Context ctx = null;
    public Timer timer = null;
	
	public TestView(Context context) {
		super(context);
		TestView.ctx = context;
		this.getHolder().addCallback(new SurfaceHolder.Callback() {

            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                Log.v(TAG, "surfaceChanged format=" + format + ", width=" + width + ", height="
                        + height);
            }

            public void surfaceCreated(SurfaceHolder holder) {
                Log.v(TAG, "surfaceCreated");
                TestView.surface = holder.getSurface();
                Rect rc = holder.getSurfaceFrame();
                
                TestLib.init(rc.width(), rc.height(), TestView.ctx.getAssets(), TestView.surface);
                timer = new Timer();
                timer.schedule(new IdleTask(), 50, 1);
            }

            public void surfaceDestroyed(SurfaceHolder holder) {
                Log.v(TAG, "surfaceDestroyed");
                
                TestLib.quit();
            }

        });
	}

}
