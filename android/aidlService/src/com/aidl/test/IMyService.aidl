package com.aidl.test;
import com.aidl.test.Student;

interface IMyService
{
	Map getMap(in String test_class,in Student student);
	Student getStudent();
}