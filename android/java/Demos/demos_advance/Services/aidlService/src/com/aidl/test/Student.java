package com.aidl.test;

import android.os.Parcel;
import android.os.Parcelable;

public class Student implements Parcelable {

	private int age;
	private String name;

	public int getAge() {
		return age;
	}

	public void setAge(int age) {
		this.age = age;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public static final Parcelable.Creator<Student> CREATOR = new Creator<Student>() {

		@Override
		public Student[] newArray(int size) {
			// TODO Auto-generated method stub
			return new Student[size];
		}

		@Override
		public Student createFromParcel(Parcel source) {
			// TODO Auto-generated method stub
			return new Student(source);
		}
	};

	public Student() {
	}

	public Student(Parcel pl) {
		age = pl.readInt();
		name = pl.readString();
	}

	@Override
	public int describeContents() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
		// TODO Auto-generated method stub
		dest.writeInt(age);
		dest.writeString(name);
	}

}
