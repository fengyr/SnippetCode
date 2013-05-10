/**
 * @file       Thumbnail.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-17 上午11:00:56 
 */

package com.pateo.as.photo.model;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * @Synopsis    Thumbnail image of an abstract class, need to achieve Parcelable interface,
 *              the object is passed between multiple Activity .
 *
 * @member THUMB_ID: Thumbnail index in the database.
 * @member IMAGE_ID: Real picture in the database index.
 * @member PATH:     Thumbnail file path.
 * @member WIDTH:    The width of the thumbnail.
 * @member HEIGHT:   The height of the thumbnail.
 */
public class PhotoThumbnail implements Parcelable {
	public int THUMB_ID;
	public int IMAGE_ID;
	public String PATH;
	public int WIDTH;
	public int HEIGHT;
	
	public PhotoThumbnail() {
	}

	public static final Parcelable.Creator<PhotoThumbnail> CREATOR = new Creator<PhotoThumbnail>() {  
        public PhotoThumbnail createFromParcel(Parcel source) {  
        	PhotoThumbnail mPhotoThumbnail = new PhotoThumbnail();  
        	mPhotoThumbnail.THUMB_ID = source.readInt();
        	mPhotoThumbnail.IMAGE_ID = source.readInt();
        	mPhotoThumbnail.PATH = source.readString();
        	mPhotoThumbnail.WIDTH = source.readInt();
        	mPhotoThumbnail.HEIGHT = source.readInt();
            
            return mPhotoThumbnail;  
        }

		@Override
		public PhotoThumbnail[] newArray(int size) {
			return new PhotoThumbnail[size];
		}  
    };
    
	@Override
	public int describeContents() {
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeInt(THUMB_ID);
		dest.writeInt(IMAGE_ID);
		dest.writeString(PATH);
		dest.writeInt(WIDTH);
		dest.writeInt(HEIGHT);
	}
}
