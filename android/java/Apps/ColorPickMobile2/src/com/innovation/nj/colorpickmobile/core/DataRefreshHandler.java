/**
 * @file       DataRefreshHandler.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2013-9-30 下午07:17:16 
 */

package com.innovation.nj.colorpickmobile.core;

import com.innovation.nj.colorpickmobile.model.ProtobufData;

/**
 *
 */
public interface DataRefreshHandler {
	void refresh(ProtobufData data);
}
