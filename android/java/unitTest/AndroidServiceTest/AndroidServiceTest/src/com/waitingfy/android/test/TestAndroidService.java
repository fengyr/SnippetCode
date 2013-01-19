package com.waitingfy.android.test;

import com.waitingfy.android.AndroidService;

import android.content.Intent;
import android.test.ServiceTestCase;
import android.test.suitebuilder.annotation.SmallTest;

public class TestAndroidService extends ServiceTestCase<AndroidService>{



	// public TestAndroidService(Class<AndroidService> serviceClass) {
	// super(serviceClass);
	// // TODO Auto-generated constructor stub
	// }
	public TestAndroidService() {
		super(AndroidService.class);
	}
	
    @Override
	protected void setUp() throws Exception {
		super.setUp();
        getContext().startService(new Intent(getContext(), AndroidService.class));
	}
    @SmallTest
    public void testSomething() {
		assertEquals(2, 2);
	}
	@Override
	protected void tearDown() throws Exception {
		getContext().stopService(new Intent(getContext(), AndroidService.class));
	}
}
