package com.foxconn.supernote.email.database;

import java.util.ArrayList;
import java.util.List;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

import com.foxconn.supernote.email.database.AccountDatabaseHelper.Tables;
import com.foxconn.supernote.email.database.AccountDatabaseHelper.UserInfo;
import com.foxconn.supernote.email.database.AccountInfo;

/**
 *  
 */
public class InterViewSQLite implements Tables, UserInfo, IAccountManagerHelper {

    private Context mContext;
    private SQLiteDatabase db;
    private ArrayList<AccountInfo> mInfoSet;
    // private ArrayList<AccountInfo> mSendInfoSet;
    private AccountInfo mEachInfo;
    final static String TAG = "InterViewSQLite-------------->";
    private static final int U = 1000;
    private static final int E = 1001;

    public InterViewSQLite(Context context) {
        mContext = context;
        db = AccountDatabaseHelper.getInstance(mContext).getWritableDatabase();
        IntitalDatabase();
    }

    /*
     * 检查数据是否在数据库的状态,并更新数据库状态
     */
    private void IntitalDatabase() {
        Log.d(TAG, "---------------int1------------->");
        ContentValues cctv = new ContentValues();
        if (db.query(EMAIL, null, null, null, null, null, null).getCount() == 0) {
            Log.d(TAG, "---------------int2------------->");
            cctv.put(ADDRESS, "gmail.com");
            cctv.put(SMTP, "smtp.gmail.com");
            cctv.put(POP3, "pop3.gmail.com");
            cctv.put(PORT1, 465);
            cctv.put(PORT2, 995);
            cctv.put(SATA, 1);
            db.insert(EMAIL, null, cctv);
            cctv.clear();
            cctv.put(ADDRESS, "163.com");
            cctv.put(SMTP, "smtp.163.com");
            cctv.put(POP3, "pop3.163.com");
            cctv.put(PORT1, 25);
            cctv.put(PORT2, 995);
            cctv.put(SATA, 2);
            db.insert(EMAIL, null, cctv);
            cctv.put(ADDRESS, "sina.com");
            cctv.put(SMTP, "smtp.sina.com");
            cctv.put(POP3, "pop3.sina.com");
            cctv.put(PORT1, 25);
            cctv.put(PORT2, 995);
            cctv.put(SATA, 3);
            db.insert(EMAIL, null, cctv);
        }
    }

    protected boolean checkStateAndUpdate(AccountInfo info) {
        DataHandler DH = new DataHandler(1000, info);
        // mInfoSet.clear();
        if (DH.getDataRow() != null) {

            Cursor c = db.query(USER, null, null, null, null, null, LASTLOGIN);
            if (c.getCount() == 0) {
                // 数据庫为空,直接增加该条数据
                db.insert(USER, null, DH.getDataRow());
                Log.v(TAG, "-------------------->add(Intital):"
                        + info.strUserName +   ";"
                        + info.nType);
                return true;
            } else {

                // 数据库存在数据
                Cursor cq = db.query(USER, null, UserInfo.USERNAME + "=" + "'"
                        + info.strUserName + "'" + " AND " + UserInfo.TYPE
                        + "=" + info.nType, null, null, null, null, null);
                if (cq.getCount() == 0) {
                    db.insert(USER, null, DH.getDataRow());
                    Log.v(TAG, "-------------------->add(First):"
                            + info.strUserName + ";"
                            + info.nType);
                } else {
                    // 发現数据库中存在提交的数据
                    // 以最近提交的为准,更新该条記录
                    int i = db.update(USER, DH.getDataRow(), UserInfo.USERNAME
                            + "=" + "'" + info.strUserName + "'" + " AND "
                            + UserInfo.TYPE + "=" + info.nType, null);
                    Log.v(TAG, "-------------------->update:"
                            + info.strUserName +  ";"
                            + info.nLastLogin + ";" + info.nType);
                }
                return true;
            }
        } else {
            db.close();
            return false;
        }
    }

    public class DataHandler extends DataRow {

        public DataHandler(int tableNum, AccountInfo mEachInfo) {
            super(tableNum, mEachInfo);
        }

        public DataHandler(int tableNum) {
            super(tableNum);
        }

        @Override
        public ArrayList<AccountInfo> PackageData() {
            mInfoSet = new ArrayList<AccountInfo>();

            Cursor c = db.query(USER, null, null, null, null, null, LASTLOGIN 
                    + " desc");

            if (!c.moveToFirst()) {
                Log.v(TAG,
                        "-------------------->PackageData()1:" + c.getCount());
                mInfoSet = null;
                return mInfoSet;
            } else {
                Log.v(TAG,
                        "-------------------->PackageData()2:" + c.getCount());
                for (c.moveToFirst(); !c.isAfterLast(); c.moveToNext()) {
                    AccountInfo info = new AccountInfo();
                    info.strUserName = c.getString(c.getColumnIndex(USERNAME));

                    info.strPassword = c.getString(c.getColumnIndex(PASSWORD));

                    info.nLastLogin = c.getLong(c.getColumnIndex(LASTLOGIN));

                    info.nType = c.getInt(c.getColumnIndex(TYPE));

                    if (info.nType == 1) {
                        info.strMailAddress = "gmail.com";
                    } else if (info.nType == 2) {
                        info.strMailAddress = "163.com";
                    } else if (info.nType == 3) {
                        info.strMailAddress = "sina.com";
                    }

                    Log.v(TAG, "-------------------->info.strUserName:"
                            + info.strUserName);

                    mInfoSet.add(info);
                }
            }
            Log.v(TAG, "-----------------------total:" + mInfoSet.size());
            return mInfoSet;
        }

    }

    @Override
    public boolean addUser(AccountInfo info) {
        boolean addToast = checkStateAndUpdate(info);
        Log.v(TAG, "------------------->add:" + addToast);
        return addToast;
    }

    @Override
    public boolean deleteUser(AccountInfo info) {
        if (info != null) {
            long d = db.delete(USER, USERNAME + "=" + "'" + info.strUserName
                    + "' AND "+TYPE +"="+"'"+info.nType+"'", null);
            Log.v(TAG, "------------------->del:" + info.strUserName + "," + d);
            return true;
        }
        return false;
    }

    @Override
    public List<AccountInfo> queryAllUsers() {
        DataHandler DH = new DataHandler(U);
        DH.PackageData();
        if (null == mInfoSet) {
            Log.v(TAG, "------------------->null:");
        } else {
            for (int i = 0; i < mInfoSet.size(); i++) {
                Log.v(TAG, "mAccountInfoList[" + i + "] = "
                        + mInfoSet.get(i).strUserName + "@"
                        + mInfoSet.get(i).strMailAddress);
            }
        }
        return mInfoSet;

    }

    @Override
    public AccountInfo getLastUser() {
        queryAllUsers();
        if (mInfoSet!=null) {
            mEachInfo = mInfoSet.get(0);
            Log.v(TAG, "------------------->getLastUser:"
                    + mEachInfo.strUserName + ",type:" + mEachInfo.nType
                    + ",mailaddress:" + mEachInfo.strMailAddress);
            if (mEachInfo.strUserName != null) {
                Cursor c = db.query(EMAIL, null, SATA + "=" + mEachInfo.nType,
                        null, null, null, null);
                if (c.moveToFirst()) {
                    mEachInfo.strMailAddress = c.getString(c
                            .getColumnIndex(ADDRESS));
                    mEachInfo.strSmtp = c.getString(c.getColumnIndex(SMTP));
                    mEachInfo.strPop3 = c.getString(c.getColumnIndex(POP3));
                    mEachInfo.nPort1 = c.getInt(c.getColumnIndex(PORT1));
                    mEachInfo.nPort2 = c.getInt(c.getColumnIndex(PORT2));
                }
                Log.v(TAG, "------------------->the last user+Emiail:"
                        + mEachInfo.strUserName + "-"
                        + mEachInfo.strMailAddress);
            }
            return mEachInfo;
        }

        return null;
    }

    @Override
    public boolean setLastUser(AccountInfo info) {
        if (info == null) {
            Log.v(TAG, "------------------->set last user:null");
            return false;
        } else{
            Log.v(TAG, "------------------->set last user:"+info.strUserName);
            checkStateAndUpdate(info);
        }
        return true;
    }
}
