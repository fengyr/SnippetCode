package jtest;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class JunitTestSuite {
    static TestSuite suite = new TestSuite();

    public static TestSuite getTestSuite() {
        TestSuite suite = new TestSuite();
        suite.addTestSuite(JunitTest.class);

        return suite;
    }
}
