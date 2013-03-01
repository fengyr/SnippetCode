package com.foxconn.edu_bbs.ota;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class BootCompleteMointor extends BroadcastReceiver {
	public void onReceive(Context context, Intent intent) {
		if (intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)) {
			Log.e("BootCompleteMointor", "BootCompleteMointor startService");
			Intent in = new Intent(context, AlertService.class);
			in.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			context.startService(in);
		}
	}
}
