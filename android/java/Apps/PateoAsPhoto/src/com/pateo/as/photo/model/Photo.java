/**
 * @file       Photo.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-17 上午11:00:26 
 */

package com.pateo.as.photo.model;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * @Synopsis Photo Image abstract class, which need to achieve Parcelable interface 
 *           so that the object is passed between the plurality Activity.
 *
 * @member ID: image in a database of the index.
 * @member PATH : Image file path.
 * @member MIME: picture type information.
 * @member BUCKET_NAME : where the folder name.
 * @member GPS_LATITUDE : GPS latitude, int type.
 * @member GPS_LONGITUDE : GPS longitude information, int type.
 * @member GPS_LATITUDE_STR : GPS Latitude, string type.
 * @member GPS_LONGITUDE_STR : GPS longitude information, the string type.
 */
public class Photo implements Parcelable {
	public int ID;
	public String PATH;
	public String MIME;
	public String BUCKET_NAME;
	public int GPS_LATITUDE;
	public int GPS_LONGITUDE;
	public String GPS_LATITUDE_STR;
	public String GPS_LONGITUDE_STR;

	public Photo() {
	}

	public String toString() {
		return String.format("%d, %s, %d, %d, %s, %s", ID, PATH, GPS_LATITUDE,
				GPS_LONGITUDE, GPS_LATITUDE_STR, GPS_LONGITUDE_STR);
	}

	public static final Parcelable.Creator<Photo> CREATOR = new Creator<Photo>() {
		public Photo createFromParcel(Parcel source) {
			Photo mPhoto = new Photo();
			mPhoto.ID = source.readInt();
			mPhoto.PATH = source.readString();
			mPhoto.MIME = source.readString();
			mPhoto.BUCKET_NAME = source.readString();
			mPhoto.GPS_LATITUDE = source.readInt();
			mPhoto.GPS_LONGITUDE = source.readInt();
			mPhoto.GPS_LATITUDE_STR = source.readString();
			mPhoto.GPS_LONGITUDE_STR = source.readString();

			return mPhoto;
		}

		@Override
		public Photo[] newArray(int size) {
			return new Photo[size];
		}
	};

	@Override
	public int describeContents() {
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeInt(ID);
		dest.writeString(PATH);
		dest.writeString(MIME);
		dest.writeString(BUCKET_NAME);
		dest.writeInt(GPS_LATITUDE);
		dest.writeInt(GPS_LONGITUDE);
		dest.writeString(GPS_LATITUDE_STR);
		dest.writeString(GPS_LONGITUDE_STR);
	}
}
