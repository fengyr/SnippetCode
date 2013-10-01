package com.innovation.nj.colorpickmobile.activity;

import java.io.UnsupportedEncodingException;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;

import ColorSepServer.Image.ImagePackage;
import android.content.res.Resources;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;

import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;
import com.innovation.nj.colorpickmobile.R;
import com.innovation.nj.colorpickmobile.core.DataReceiver;
import com.innovation.nj.colorpickmobile.view.MainPageFragment;
import com.innovation.nj.colorpickmobile.view.TabsFragment;
import com.viewpagerindicator.TabPageIndicator;

public class MainActivity extends FragmentActivity {
	static private final String TAG = "MainActivity";
	private static String[] TABS_TITLE;

	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {

		@Override
		public void onManagerConnected(int status) {
			switch (status) {
			case LoaderCallbackInterface.SUCCESS: {
				Log.i(TAG, "OpenCV loaded successfully");
				break;
			}

			default: {
				super.onManagerConnected(status);
				break;
			}
			}
		}
	};
	
	@Override
	protected void onResume() {
		super.onResume();
		
		OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_6, this, mLoaderCallback);
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().requestFeature(Window.FEATURE_NO_TITLE);

		Resources res = getResources();
		TABS_TITLE = new String[] { res.getString(R.string.main_page),
				res.getString(R.string.class_page) };

		setContentView(R.layout.main_ui);

		FragmentPagerAdapter adapter = new MainUiAdapter(
				getSupportFragmentManager());

		ViewPager pager = (ViewPager) findViewById(R.id.pager);
		pager.setAdapter(adapter);

		TabPageIndicator indicator = (TabPageIndicator) findViewById(R.id.indicator);
		indicator.setViewPager(pager);

		test_protobuf();
		test_receiver();
	}

	private void test_receiver() {
	}

	private void test_protobuf() {
		ImagePackage.Builder builder = ImagePackage.newBuilder();
		builder.setTime(1234);
		builder.setFrameCount(1000000);
		builder.setImageClass(1);
		builder.setClassCount(10);
		builder.setImageWidth(500);
		builder.setImageHeight(600);
		builder.setProcessStatus(101);
		builder.setProcessTime(20000);
		builder.setRetinueInit(1);
		builder.setAddSampleResult(2);
		builder.setDataType(1);
		builder.setAddSampleCounts("1,2,3,4,");
		ByteString data = null;
		try {
			data = ByteString.copyFrom("hello", "utf-8");

		} catch (UnsupportedEncodingException e1) {
			e1.printStackTrace();
		}
		builder.setImageData(data);
		ImagePackage image = builder.build();
		byte[] buf = image.toByteArray();

		try {
			ImagePackage unpackage = ImagePackage.parseFrom(buf);
			Log.e(TAG, "getImageClass=" + unpackage.getImageClass()
					+ " getAddSampleCounts=" + unpackage.getAddSampleCounts()
					+ " getFrameCount=" + unpackage.getFrameCount());
		} catch (InvalidProtocolBufferException e) {
			e.printStackTrace();
		}
	}

	class MainUiAdapter extends FragmentPagerAdapter {
		public MainUiAdapter(FragmentManager fm) {
			super(fm);
		}

		@Override
		public Fragment getItem(int position) {
			Fragment fragment;
			int id = position % TABS_TITLE.length;

			switch (position) {
			case 0: // 主页
				fragment = MainPageFragment.newInstance(TABS_TITLE[id]);
				break;
			case 1: // 分类信息
				fragment = TabsFragment.newInstance(TABS_TITLE[id]);
				break;
			default:
				fragment = MainPageFragment.newInstance(TABS_TITLE[id]);
				break;
			}

			return fragment;
		}

		@Override
		public CharSequence getPageTitle(int position) {
			return TABS_TITLE[position % TABS_TITLE.length].toUpperCase();
		}

		@Override
		public int getCount() {
			return TABS_TITLE.length;
		}
	}
}
