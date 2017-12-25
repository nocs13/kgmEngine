package com.kgmEngine.Game;

import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.graphics.Rect;
import android.os.SystemClock;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class GameView extends SurfaceView
{
	public class IdleTask extends TimerTask{
		@Override
		public void run(){
			//Log.v(TAG, "idle");
			GameLib.idle();
		}
	}
	
	private static String TAG = "TestView";
	public static Surface surface = null;
    public static Context ctx = null;
    public Timer timer = null;
	
	public GameView(Context context) {
		super(context);
		GameView.ctx = context;
		this.getHolder().addCallback(new SurfaceHolder.Callback() {

            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                Log.v(TAG, "surfaceChanged format=" + format + ", width=" + width + ", height="
                        + height);
            }

            public void surfaceCreated(SurfaceHolder holder) {
                Log.v(TAG, "surfaceCreated");
                GameView.surface = holder.getSurface();
                Rect rc = holder.getSurfaceFrame();
                
                GameLib.init(rc.width(), rc.height(), GameView.ctx.getAssets(), GameView.surface);
                timer = new Timer();
                timer.schedule(new IdleTask(), 50, 1);
            }

            public void surfaceDestroyed(SurfaceHolder holder) {
                Log.v(TAG, "surfaceDestroyed");
                
                GameLib.quit();
            }

        });
	}

}
