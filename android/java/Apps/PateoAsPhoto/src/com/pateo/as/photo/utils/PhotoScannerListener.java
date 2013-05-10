/**
 * @file       PhotoScannerListener.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-16 下午05:57:10 
 */

package com.pateo.as.photo.utils;

import com.pateo.as.photo.model.PhotoAlbums;

/**
 *
 */
public interface PhotoScannerListener {
	// The scanner has scan completed and return the available images.
	static final int FLAG_SCAN_COMPLETED = 0;
	// The scanner has scan completed, but has no available images.
	static final int FLAG_NO_AVAILABLE_IMG = 1;
	
	// callback when scanner work finished.
	public void onScannerFinished(int flag, PhotoAlbums albums);
}
