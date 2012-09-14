/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: F:\\Android API DEMO\\aidlService\\src\\com\\aidl\\test\\IMyService.aidl
 */
package com.aidl.test;

public interface IMyService extends android.os.IInterface {
	/** Local-side IPC implementation stub class. */
	public static abstract class Stub extends android.os.Binder implements
			com.aidl.test.IMyService {
		private static final java.lang.String DESCRIPTOR = "com.aidl.test.IMyService";

		/** Construct the stub at attach it to the interface. */
		public Stub() {
			this.attachInterface(this, DESCRIPTOR);
		}

		/**
		 * Cast an IBinder object into an com.aidl.test.IMyService interface,
		 * generating a proxy if needed.
		 */
		public static com.aidl.test.IMyService asInterface(
				android.os.IBinder obj) {
			if ((obj == null)) {
				return null;
			}
			android.os.IInterface iin = (android.os.IInterface) obj
					.queryLocalInterface(DESCRIPTOR);
			if (((iin != null) && (iin instanceof com.aidl.test.IMyService))) {
				return ((com.aidl.test.IMyService) iin);
			}
			return new com.aidl.test.IMyService.Stub.Proxy(obj);
		}

		public android.os.IBinder asBinder() {
			return this;
		}

		@Override
		public boolean onTransact(int code, android.os.Parcel data,
				android.os.Parcel reply, int flags)
				throws android.os.RemoteException {
			switch (code) {
			case INTERFACE_TRANSACTION: {
				reply.writeString(DESCRIPTOR);
				return true;
			}
			case TRANSACTION_getMap: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				Student _arg1;
				if ((0 != data.readInt())) {
					_arg1 = Student.CREATOR.createFromParcel(data);
				} else {
					_arg1 = null;
				}
				java.util.Map _result = this.getMap(_arg0, _arg1);
				reply.writeNoException();
				reply.writeMap(_result);
				return true;
			}
			case TRANSACTION_getStudent: {
				data.enforceInterface(DESCRIPTOR);
				Student _result = this.getStudent();
				reply.writeNoException();
				if ((_result != null)) {
					reply.writeInt(1);
					_result
							.writeToParcel(
									reply,
									android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
				} else {
					reply.writeInt(0);
				}
				return true;
			}
			}
			return super.onTransact(code, data, reply, flags);
		}

		private static class Proxy implements com.aidl.test.IMyService {
			private android.os.IBinder mRemote;

			Proxy(android.os.IBinder remote) {
				mRemote = remote;
			}

			public android.os.IBinder asBinder() {
				return mRemote;
			}

			public java.lang.String getInterfaceDescriptor() {
				return DESCRIPTOR;
			}

			public java.util.Map getMap(java.lang.String test_class,
					Student student) throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				java.util.Map _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(test_class);
					if ((student != null)) {
						_data.writeInt(1);
						student.writeToParcel(_data, 0);
					} else {
						_data.writeInt(0);
					}
					mRemote.transact(Stub.TRANSACTION_getMap, _data, _reply, 0);
					_reply.readException();
					java.lang.ClassLoader cl = (java.lang.ClassLoader) this
							.getClass().getClassLoader();
					_result = _reply.readHashMap(cl);
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			public Student getStudent() throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				Student _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					mRemote.transact(Stub.TRANSACTION_getStudent, _data,
							_reply, 0);
					_reply.readException();
					if ((0 != _reply.readInt())) {
						_result = Student.CREATOR.createFromParcel(_reply);
					} else {
						_result = null;
					}
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}
		}

		static final int TRANSACTION_getMap = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
		static final int TRANSACTION_getStudent = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
	}

	public java.util.Map getMap(java.lang.String test_class, Student student)
			throws android.os.RemoteException;

	public Student getStudent() throws android.os.RemoteException;
}
