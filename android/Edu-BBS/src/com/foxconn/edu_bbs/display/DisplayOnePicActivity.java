package com.foxconn.edu_bbs.display;

import java.io.File;
import java.util.Timer;
import java.util.TimerTask;

import com.foxconn.edu_bbs.ota.R;
import com.foxconn.edu_bbs.utils.BBSUtilities;
import com.foxconn.edu_bbs.utils.TransConfig;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

public class DisplayOnePicActivity extends Activity {

	static private final String TAG = "DisplayOnePicActivity";
	private TransConfig mConfig = TransConfig.getInstance();
	private String targetFileDir = mConfig.getTargetDir();
	private String fileNamePart = "-bbs-0-%d.jpg";

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,  
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		setContentView(R.layout.main);
		
		task();
	}
	
	public void task() {
		Timer timer = new Timer();
		timer.schedule(new TimerTask() {

			@Override
			public void run() {
				refresh();
			}
		}, 0, 5000);
	}

	Handler hand = new Handler() {
	};

	public void refresh() {
		hand.post(new Runnable() {
			@Override
			public void run() {
				String device_uid = new BBSUtilities().getMasterID();
				String file = targetFileDir + "/" + device_uid + String.format(fileNamePart, mConfig.getDeviceNumber());
				Log.e(TAG, "!!!!!!!!!!!!! refresh targetFile = " + file);
				ImageView iv = (ImageView) findViewById(R.id.loadPic);
				
				File mFile = null;
				mFile = new File(file);
				if (!mFile.exists()) {
//					Toast.makeText(this, "No local data, start to download", Toast.LENGTH_LONG).show();
				} else {
					Bitmap bitmap = BitmapFactory.decodeFile(file);
					iv.setImageBitmap(bitmap);
//					Toast.makeText(this, "OK, Show the first picture '0'!", Toast.LENGTH_LONG).show();
				}
			}
		});
	}
}
