package test;

import string.Contain;
import typetest.ListTestTest;
import sort.SortElement;
import sort.SortExample;
import sort.SortExample2;
import sax.SaxDemo;
import jtest.JunitTest;
import thread.ThreadDemo;
import thread.SumCal;
import concurrentTest.ExcutorEx;
import concurrentTest.FutureTest;
import concurrentTest.BarrierTest;
import generalType.GenericTest;
import generalType.GenericPatten;
import argList.ArgListTest;

import java.lang.Runnable;
import java.nio.charset.Charset;
import java.io.UnsupportedEncodingException;
import java.io.File;

import javax.xml.parsers.SAXParserFactory;
import javax.xml.parsers.SAXParser;

import java.util.List;
import java.util.ArrayList;
import java.util.regex.*;

import junit.framework.TestSuite;
import junit.textui.TestRunner;

public class TestMain {

    //列表测试
    static private void test_list() {
        ListTestTest test1 = new ListTestTest();
        test1.PrintList();
        ListTestTest test2 = test1;
        test2.mark = 1;
        test1.PrintList();
        test2.PrintList();
    }

    //编码测试
    static private void test_encode() {
        String str1 = "你好";
        System.out.println(str1);

        try {
            String str2 = new String(str1.getBytes(), "GBK");
            System.out.println(str2);

            String str3 = new String(str2.getBytes("GBK"), "UTF-8");
            System.out.println(str3);
        }
        catch(Exception e) {
            System.out.println(e.getMessage());
        }

    }

    //正则表达式测试
    static private void test_reg() {
        String regEx = "^([0-9+]+)$";

        String str = "asdja021212asjda2932";

        Pattern p = Pattern.compile(regEx);

        Matcher m = p.matcher(str);

        boolean rs = m.find();
        if ( rs ) {
            System.out.println("find it");

            for(int i=1; i<=m.groupCount(); i++){
                System.out.println(m.group(i));
            }
        }
        else {
            System.out.println("not find");
        }
    }

    //排序测试
    static private void test_compare() {
        List<SortElement> list = new ArrayList<SortElement>(); 
        list.add(new SortElement("zenki", 28));
        list.add(new SortElement("zenki2", 229));
        list.add(new SortElement("abc3", 330));
        list.add(new SortElement("dcbi3", 130));
        list.add(new SortElement("xmy3", 99));

        // SortExmaple2
        // 抽象化比较类型和比较方法
        /*
        System.out.println("按名字排序");
        new SortExample2().sort(list, SortElement.sortName);

        for (SortElement e : list) {
            System.out.println(e.getName() + ":" + e.getId());
        }
        */

        System.out.println("按名字排序");
        new SortExample().sortByName(list);

        for (SortElement e : list) {
            System.out.println(e.getName() + ":" + e.getId());
        }

        System.out.println("\n按ID排序");
        new SortExample().sortById(list);

        for (SortElement e : list) {
            System.out.println(e.getName() + ":" + e.getId());
        }
    }

    // 字符串测试
    static private void test_string() {
        Contain contain = new Contain();

        if ( contain.containIt(";") ) {
            System.out.println("contain it");
        }
        else {
            System.out.println("not contain it");
        }
    }

    // sax读取xml
    static private void test_sax() {
        SAXParserFactory spf = SAXParserFactory.newInstance();
        //XMLReader xmlReader = null;
        SAXParser saxParser = null;

        try {
            saxParser = spf.newSAXParser();
            //xmlReader = saxParser.getXMLReader();
        }
        catch(Exception e) {
            System.err.println(e);
        }

        try {
            saxParser.parse(new File("../src/demo.xml"), new SaxDemo());
        }
        catch(Exception e) {
            System.err.println(e);
        }
    }

    // Junit测试
    static private void test_Junit() {
        junit.textui.TestRunner.run(JunitTest.suite());
    }

    // 线程测试
    static private void test_Thread() {
        SumCal cal = new SumCal();
        SumCal cal2 = new SumCal();

        ThreadDemo demo1 = new ThreadDemo("thread1", cal, cal2);
        ThreadDemo demo2 = new ThreadDemo("thread2", cal, cal2);

        demo1.start();
        demo2.start();
    }

    // concurrent库测试
    static private void test_Excutor() {
        ExcutorEx ee = new ExcutorEx(10);
        ee.prepareThreads();

        try {
            Thread.sleep(1000);
            ee.start();
            Thread.sleep(2000);
            ee.shutDown();          // 此处不调用shutDown，线程不会销毁
        } catch(Exception e) {
        }
    }

    static private void test_Future() {
        FutureTest ft = new FutureTest();
        ft.submitTask();
    }

    // 泛型测试
    static private void test_Generic() {
        GenericTest<String> gt1 = new GenericTest<String>("hello world");
        gt1.showObjClass();
        gt1.showTypeInfo();

        GenericTest<File> gt2 = new GenericTest<File>(new File("/home/zenki"));
        gt2.showObjClass();
        String info = gt2.showTypeInfo();
        System.out.println("test_Generic gt2 info = " + info);
    }

    // 泛型通配符
    static private void test_GenericPatten() {
        GenericPatten gp = new GenericPatten();
        gp.showFruit();
    }

    static private void test_Barrier() {
        BarrierTest test1 = new BarrierTest(1000);
        BarrierTest test2 = new BarrierTest(2000);

        test1.start();
        test2.start();
        
        test1.reset();
        test2.reset();
    }

    // 可变参数测试
    static private void test_ArgList() {
        ArgListTest alt = new ArgListTest();
        alt.showArgInfo("hello", "world", 1, "!");
    }
	
	public static void main(String[] argvs) throws UnsupportedEncodingException{
        test_GenericPatten();
    }
}
