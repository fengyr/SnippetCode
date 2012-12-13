package org.example.test;

import android.test.ActivityInstrumentationTestCase;

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
public class .MyTestTest extends ActivityInstrumentationTestCase<.MyTest> {

    private static final String TAG = "MyTestTest";

    public .MyTestTest() {
        super("org.example.test", .MyTest.class);
    }

    public void testDemo() {
    }
}
