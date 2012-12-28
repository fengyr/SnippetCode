/**
 * @file       IAccountManagerHelper.java
 *
 * @revision:  none 
 *
 * @version    0.0.01
 * @author:    Zenki (zhajun), zenki2001cn@163.com
 * @date:      2012-3-2 上午08:21:29 
 */

package com.foxconn.supernote.email.database;

import java.util.List;

import com.foxconn.supernote.email.database.AccountInfo;

/**
 * This Information is show how R/W the database
 * 
 * @author: Mark
 * @date: 2012-3-7 PM05:21:29
 * 
 *        InterViewSQLite ivs = new InterViewSQLite(this.context);
 *        AccountInfo info; 
 * 
 *        add:            boolean add =ivs.addUser(info); 
 *        delete:         boolean del =ivs.deleteUser(info); 
 *        queryAllUser:   List<AccountInfo> aIList =ivs.queryAllUsers();
 *        getLastUser:    AccountInfo aI = ivs.getLastUser();
 *        setLastUser:    boolean set = ivs.setLastUser(info);
 * 
 *         
 *       Account info  should contain: info.strUserName;(not null)
 *                                     info.strPassword;(not null) 
 *                                     info.strType; (not null)
 *                                     info.nLastLogin;(not null)
 *                                     (others see AccountInfo.java )
 * 
 */
public interface IAccountManagerHelper {
    /*
     * 提供新增一个用戶,调用该方法
     *  
     * 
     * @return true-success;false-fail;
     */
    public boolean addUser(AccountInfo info);

    /*
     * 删除指定用戶
     * 
     * @return true-success;false-fail;
     */
    public boolean deleteUser(AccountInfo info);

    /*查詢所有用戶
     *
     *
     * @return null-当前用戶數据表为空, List<AccountInfo>-返回类型为所有用戶信息对象的数组
     */
    public List<AccountInfo> queryAllUsers();

    /*查詢出最近设置的用戶
     * 
     *  @return null-当前用戶數据表为空, AccountInfo-返回一个包含最近用戶信息的对象
     */
    public AccountInfo getLastUser();

    /*指定一个用戶,保存为最近用戶
     * 
     * @return false- fail; true-  sucess;
     */
    public boolean setLastUser(AccountInfo info);
}

