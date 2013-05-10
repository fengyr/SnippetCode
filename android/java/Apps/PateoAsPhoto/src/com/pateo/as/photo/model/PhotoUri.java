/**
 * @file       PhotoUri.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-5-9 上午10:22:21 
 */

package com.pateo.as.photo.model;

import android.net.Uri;

/**
 *
 */
public class PhotoUri {
	public static final String AUTHORITY = "media";
	public static final String TYPE = "/images/media";
	public static final String LOCATION = "udisk2";
	public static final String CONTENT_AUTHORITY_SLASH = "content://"
			+ AUTHORITY + "/";
	public static final String URI = CONTENT_AUTHORITY_SLASH + LOCATION + TYPE;

	public static Uri getUri() {
		return Uri.parse(URI);
	}
}
