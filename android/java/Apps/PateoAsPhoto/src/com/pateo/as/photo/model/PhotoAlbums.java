/**
 * @file       Albums.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), junzha@pateo.com.cn
 * @date:      2013-4-17 上午09:50:40 
 */

package com.pateo.as.photo.model;

import java.util.ArrayList;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

/**
 * @Synopsis The PhotoAlbums contains all Albums collection.
 *
 * @member mAlbumList: Album list that contains all the picture album object.
 */
public class PhotoAlbums {
	static private final String TAG = "PhotoAlbums";
	private ArrayList<Album> mAlbumList;

    /**
     * @Synopsis Album is a picture album abstract class, which classified folder 
     * 			 that contains the folder list of all the pictures.
     *           The Parcelable interface class is required to achieve the object 
     *           so as to pass between the plurality Activity.
     *
     * @member PhotoList: pictures list of objects.
     * @member ThumbnailList: thumbnail list of objects.
     * @member BucketName: folder name.
     */
	static public class Album implements Parcelable {
		public ArrayList<Photo> PhotoList;
		public ArrayList<PhotoThumbnail> ThumbnailList;
		public String BucketName;

		public Album() {
			PhotoList = new ArrayList<Photo>();
			ThumbnailList = new ArrayList<PhotoThumbnail>();
		}
		
		public int getPhotoSize() {
			return PhotoList.size();
		}
		
		@Override
		public String toString() {
			Photo photo = PhotoList.get(0);
			String str = String.format("Path: %s - id: %d - bucket: %s", photo.PATH, photo.ID, photo.BUCKET_NAME);
			return str;
		}
		
		public static final Parcelable.Creator<Album> CREATOR = new Creator<Album>() {  
	        public Album createFromParcel(Parcel source) {  
	        	Album mAlbum = new Album();  
	        	source.readTypedList(mAlbum.PhotoList, Photo.CREATOR);
	        	source.readTypedList(mAlbum.ThumbnailList, PhotoThumbnail.CREATOR);  
	            mAlbum.BucketName = source.readString();
	            
	            return mAlbum;
	        }

			@Override
			public Album[] newArray(int size) {
				return new Album[size];
			}  
	    };  

		@Override
		public int describeContents() {
			return 0;
		}

		@Override
		public void writeToParcel(Parcel dest, int flags) {
			dest.writeTypedList(PhotoList);
			dest.writeTypedList(ThumbnailList);
			dest.writeString(BucketName);
		}
	}

	public PhotoAlbums() {
		mAlbumList = new ArrayList<PhotoAlbums.Album>();
	}

	public void addAlbum(Album album) {
		try {
			mAlbumList.add(album);
		} catch (Exception e) {
			Log.e(TAG, "addAlbum error " + e);
		}
	}

	public ArrayList<Album> getAlbumList() {
		return mAlbumList;
	}
	
	public int getAlbumSize() {
		if (mAlbumList != null) {
			return mAlbumList.size();
		}
		
		return 0;
	}
	
	public void clear() {
		mAlbumList.clear();
	}

	public void dump() {
		int size = 0;
		
		for (Album album : mAlbumList) {
			Log.d(TAG, "------------ dump photo ------------");
			
			size += album.PhotoList.size();
			for (Photo photo : album.PhotoList) {
				Log.d(TAG, "id: " + photo.ID + " path: " + photo.PATH
						+ " mime: " + photo.MIME + " bucket: "
						+ photo.BUCKET_NAME);
			}

			Log.d(TAG, "------------ dump thumbnails ------------");
			for (PhotoThumbnail thumb : album.ThumbnailList) {
				Log.d(TAG, "id: " + thumb.IMAGE_ID + " path: " + thumb.PATH
						+ " width: " + thumb.WIDTH + " height: " + thumb.HEIGHT);
			}
		}
		
		Log.d(TAG, "============ photo size: " + size + " album size: " + getAlbumSize() + " ================");
	}
}
