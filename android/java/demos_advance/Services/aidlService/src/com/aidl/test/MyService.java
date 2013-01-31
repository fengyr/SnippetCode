package com.aidl.test;

import java.util.HashMap;
import java.util.Map;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;

public class MyService extends Service {

	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return new MyServiceimpl();
	}

	// 这里也可以直接定义一个Stub类型的变量mBinder,然后在onBind()中返回这个变量供调用者使用
	// private IMyService.Stub binder = new Stub(){...}

	public class MyServiceimpl extends IMyService.Stub {

		@Override
		public Student getStudent() throws RemoteException {
			// TODO Auto-generated method stub
			Student st = new Student();
			st.setAge(18);
			st.setName("Raul");

			return st;
		}

		@Override
		public Map getMap(String testClass, Student student)
				throws RemoteException {
			// TODO Auto-generated method stub
			Map<String, Object> map = new HashMap<String, Object>();
			map.put("class", "高三");
			map.put("age", student.getAge());
			map.put("name", student.getName());
			return map;
		}

	}

}
