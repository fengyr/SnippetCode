package com.foxconn.supernote.email.database;


 

import android.R.integer;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.database.sqlite.SQLiteOpenHelper;
import android.net.Uri;
import android.provider.BaseColumns;

public class AccountDatabaseHelper extends SQLiteOpenHelper {

	private static final String TAG = "AccountDatabaseHelper";
	private static final String DATABASE_NAME = "email_account.db";
	private final Context mContext;
	public interface Tables {
		public static final String USER = "user";
		public static final String EMAIL="address";
	
		
	}
	public interface UserInfo {
		public static final String USERNAME = "user_name";
		public static final String ADDRESS="email_address";
		public static final String PASSWORD = "password";
		public static final String LASTLOGIN="last_time_login";
		public static final String SMTP="smtp";
		public static final String POP3="pop3";
		public static final String PORT1="port1";
		public static final String PORT2="port2";
		public static final String TYPE="type";
		public static final String SATA="sata";
		public static final String DELTA="delta";
		public static final String BETA="beta";
	}
	
	   private static AccountDatabaseHelper sSingleton = null;
	 

	public static synchronized AccountDatabaseHelper getInstance(Context context) {
	        if (sSingleton == null) {
	            sSingleton = new AccountDatabaseHelper(context);
	        }
	        return sSingleton;
	    }

	  public AccountDatabaseHelper(Context context) {
		  super(context, DATABASE_NAME, null, 100);
		  mContext = context;
		}
	
	@Override
	public void onCreate(SQLiteDatabase db) {
		   String sql = "CREATE  TABLE "+ Tables.USER +" ("+
		     BaseColumns._ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"+
		     UserInfo.USERNAME +" STRING DEFAULT NULL,"+		
		     UserInfo.PASSWORD+" STRING DEFAULT NULL,"+
		     UserInfo.LASTLOGIN +" INTEGER NOT NULL DEFAULT 0,"+
		     UserInfo.TYPE+" INTEGER NOT NULL DEFAULT 0"+
	          ");"; 
      db.execSQL(sql);
		String sql01 = "CREATE  TABLE "+Tables.EMAIL+ " ("+
		       BaseColumns._ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"+
		       UserInfo.ADDRESS +" TEXT,"+       
		       UserInfo.POP3+" TEXT,"+
		       UserInfo.SMTP+" TEXT,"+
		       UserInfo.PORT1+" INTEGER NOT NULL DEFAULT 0,"+
		       UserInfo.PORT2+" INTEGER NOT NULL DEFAULT 0,"+
		       UserInfo.SATA+" INTEGER NOT NULL DEFAULT 0"+
		       ");";
		db.execSQL(sql01);
	
	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		
		
	}

}