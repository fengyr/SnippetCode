package jtest;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

import string.Contain;

public class JunitTest extends TestCase {

    Contain contain = null;
    static TestSuite suite = new TestSuite();

    public JunitTest(String args) {
        super(args);
    }

    protected void setUp() {
        System.out.println("JunitTest setUp");

        contain = new Contain();
    }

    protected void tearDown() {
        System.out.println("JunitTest tearDown");

        contain = null;
    }

    public static Test suite() {
        //suite.addTestSuite(JunitTest.class);

        suite.addTest(new JunitTest("testContainIt"));

        return suite;
    }

    public void testContainIt() {
        //boolean ok = contain.containIt("=");
        boolean ok = false;

        assertEquals(false, ok);
    }

    public void testDemo() {
        boolean ok = contain.containIt("!");

        assertEquals(false, ok);
    }
}
