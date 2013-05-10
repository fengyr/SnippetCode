/**
 * @file       ToolBarController.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-5-2 上午09:56:21 
 */

package com.pateo.as.photo.view;

/**
 *
 */
public interface ToolBarAdapter {
	public void displayPrePhoto();
	public void displayNextPhoto();
	public void rotateLeftPhoto();
	public void rotateRightPhoto();
	public void navigate();
	public boolean fullScreen();
	public boolean slideMode();
	public void deletePhoto();
}
