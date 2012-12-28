package com.foxconn.supernote.email.database;

import java.util.ArrayList;

import com.foxconn.supernote.email.database.AccountDatabaseHelper.Tables;
import com.foxconn.supernote.email.database.AccountDatabaseHelper.UserInfo;

import android.content.ContentProvider;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteStatement;
import android.net.Uri;
import android.text.TextUtils;
import android.util.Log;

public abstract class DataRow implements Tables,UserInfo {

    private static final int U = 1000;
    private static final int E = 1001;
    private static ContentValues cv;

    public DataRow(int tableNum){
    }

    public DataRow(int tableNum,AccountInfo mEachInfo){

        cv = new ContentValues();

        if(tableNum == U){
            cv.put(USERNAME, mEachInfo.strUserName);
            cv.put(PASSWORD, mEachInfo.strPassword);
            cv.put(LASTLOGIN,mEachInfo.nLastLogin);
            cv.put(TYPE, mEachInfo.nType);
        }
        else if(tableNum == E){
            cv.put(ADDRESS, mEachInfo.strMailAddress);
            cv.put(USERNAME, mEachInfo.strPop3);
            cv.put(USERNAME, mEachInfo.strSmtp);
        }
    }

    public ContentValues getDataRow(){
        return cv;
    }

    public abstract ArrayList<AccountInfo> PackageData() ;
}
