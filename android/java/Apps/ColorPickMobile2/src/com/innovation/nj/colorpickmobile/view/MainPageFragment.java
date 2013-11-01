package com.innovation.nj.colorpickmobile.view;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfByte;

import com.innovation.nj.colorpickmobile.R;
import com.innovation.nj.colorpickmobile.activity.MainActivity;
import com.innovation.nj.colorpickmobile.core.DataReceiver;
import com.innovation.nj.colorpickmobile.core.DataRefreshHandler;
import com.innovation.nj.colorpickmobile.model.ProtobufData;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

public final class MainPageFragment extends Fragment implements
		DataRefreshHandler {
	static private final String TAG = "MainPageFragment";
	private static final String KEY_CONTENT = "TabsFragment:Content";

	public static final int MSG_REFRESH_PIC = 1;

	ImageView mViewMainPic;
	TextView mViewCurrentPicNum;
	DataReceiver mReceiver;

	public static MainPageFragment newInstance(String title) {
		MainPageFragment fragment = new MainPageFragment();

		return fragment;
	}

	private String mContent = "???";

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if ((savedInstanceState != null)
				&& savedInstanceState.containsKey(KEY_CONTENT)) {
			mContent = savedInstanceState.getString(KEY_CONTENT);
		}

		mReceiver = new DataReceiver(this.getActivity(), "192.168.1.101", 11014);
		mReceiver.registerRefreshHandler(this);
		mReceiver.run();
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		LinearLayout layout = (LinearLayout) inflater.inflate(
				R.layout.main_tab, null);

		mViewMainPic = (ImageView) layout.findViewById(R.id.id_main_pic);
		mViewMainPic.setImageResource(R.drawable.icon);
		mViewCurrentPicNum =  (TextView) layout.findViewById(R.id.id_current_pic_num);

		return layout;
	}

	@Override
	public void onDestroy() {
		mReceiver.stop();

		super.onDestroy();
	}

	@Override
	public void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		outState.putString(KEY_CONTENT, mContent);
	}

	private Handler H = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_REFRESH_PIC: {
				ProtobufData data = (ProtobufData) msg.obj;
				refreshPicture(data);
				break;
			}

			default:
				break;
			}
		};
	};

	private void refreshPicture(ProtobufData data) {
		Bitmap bmp = data.decodeImageData();
		String info;
		
		if (mViewMainPic != null) {
			mViewMainPic.setImageBitmap(bmp);
		}
		
		if (mViewCurrentPicNum != null) {
			info = String.format("第 %d 类", data.image_class);
			mViewCurrentPicNum.setText(info);
		}
	}

	@Override
	public void refresh(ProtobufData data) {
		Message msg = Message.obtain();
		msg.what = MSG_REFRESH_PIC;
		msg.obj = data;
		H.sendMessage(msg);
	}
}
