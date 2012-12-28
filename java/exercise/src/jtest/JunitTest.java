package jtest;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

import string.Contain;

public class JunitTest extends TestCase {

    Contain contain = null;

    public JunitTest(String args) {
        super(args);
    }

    protected void setUp() {
        try {
            super.setUp();
        } catch(Exception e) {
        }

        System.out.println("JunitTest setUp");

        contain = new Contain();
    }

    protected void tearDown() {
        System.out.println("JunitTest tearDown");

        contain = null;

        try {
            super.tearDown();
        } catch(Exception e) {
        }
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
