package org.example.test;

import android.test.ActivityInstrumentationTestCase;
import android.util.Log;

/**
 * This is a simple framework for a test of an Application.  See
 * {@link android.test.ApplicationTestCase ApplicationTestCase} for more information on
 * how to write and extend Application tests.
 * <p/>
 * To run this test, you can type:
 * adb shell am instrument -w \
 * -e class org.example.test..MyTestTest \
 * org.example.test.tests/android.test.InstrumentationTestRunner
 */
public class MyTestTest extends ActivityInstrumentationTestCase<MyTest> {

    private static final String TAG = "MyTestTest";
    private MyTest a = null;

    public MyTestTest() {
        super("org.example.test", MyTest.class);
        Log.v(TAG, " MyTestTest  ");
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        a = getActivity();

        Log.v(TAG, " setUp ");
    }

    public void testIsFinished() {
        assertTrue("activity is finished", a.isFinishing());
        Log.v(TAG, " isFinished ? ");
    }

    public void testIsNotFinished() {
        assertTrue("activity is finished", !a.isFinishing());
        Log.v(TAG, " isNotFinished ? ");
    }
}
