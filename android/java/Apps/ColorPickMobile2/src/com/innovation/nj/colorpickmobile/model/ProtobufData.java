/**
 * @file       ProtobufData.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2013-9-30 下午02:56:43 
 */

package com.innovation.nj.colorpickmobile.model;

import com.google.protobuf.ByteString;

/**
 *
 */
public class ProtobufData {
	public int time;
	public long frame_count;
	public int image_class;
	public int class_count;
	public int image_width;
	public int image_height;
	public int process_time;
	public int process_status;
	public int retinue_init;
	public int add_sample_result;
	public int data_type;
	public String add_sample_counts;
	public ByteString image_data;
}
