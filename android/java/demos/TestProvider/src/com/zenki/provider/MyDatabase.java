/**
 * @file       MyDatabase.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2010-11-24 上午09:07:24 
 */

package com.zenki.provider;

import android.provider.BaseColumns;
import android.net.Uri;

/**
 *
 */
public class MyDatabase {

	private MyDatabase() {
		
	}
	
	static public final class MyColumn implements BaseColumns {
		private MyColumn() {
		}

		public static final Uri CONTENT_URI = Uri.parse("content://com.zenki.myprovider/myprovider");
		public static final String CONTENT_TYPE = "vnd.Android.cursor.dir";
		public static final String CONTENT_ITEM_TYPE = "vnd.Android.cursor.item";
		public static final String NAME = "name";
		public static final String AGE = "age";
	}
}
