#ifndef _READDEPLOY_H_
#define _READDEPLOY_H_

#ifndef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifndef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#ifndef HAVE_REGEX_H
#include <regex.h>
#endif

/*
 *本函数读出配置文件内信息
 *提供读取文件头部信息及每个文件内部参数信息
 *
 */

/*
 *
 */
#define AUTHSH                "servlet/MatServer?type=HTTPGetCameraOfSiteV2\\&siteID="

/*
 *文件名定义
 */
#define FILENAME              "/root/.nmp.txt"

/*
 *文件读取状态定义
 */
#define FLAG_ERROR            -1
#define FLAG_HEAD             0
#define FLAG_OVER             1

/*
 *极限值定义
 */
#define ALLMAX                256

typedef enum pro
{
     Na,
     Pel_p,
     Dmd,
     Ktd
}PTC;

/*
 *结构定义
 */
typedef struct headinformation
{
     char *stationname;                //站名
     int  id;                          //站ID
     char *addr;                       //报警组播地址
     int  port;                        //报警组播端口
     struct headinformation *next;     //链表指针
}HEADINF;

typedef struct allinformation
{
     int  quantity;                    //信息数量
     int  *id;                         //球机ID
     char **name;                      //球机位置名
     char **videoaddr;                 //视频组播地址
     int  *videoport;                  //视频端口
     int  *controlport;                //控制端口
     int  *dmduid;                     //dmduID
     int  *alarmid;                    //报警ID
     int  *panaddr;                    //云台地址
     PTC  *protocol;              //球机协议
     char **type;                      //控制卡类型名称
}ALLINF;

/*
 *函数定义
 */

/*
 *
 */
int get_deploy(char *ip, char *id);

/*
 *本函数读出站点名，站点ID，站点报警组播IP地址，站点报警组播IP对应端口
 *返回-1表示读取错误，返回0表示读取了空数据，返回1表示读取成功，
 */
int deploy_head(HEADINF **inf);
/*
 *释放HEADINF结构的空间
 */
int free_head(HEADINF **inf);

/*
 *本函数读取1、球机ID，
 *         2、球机位置名，
 *         3、球机组播地址，
 *         4、视频端口，
 *         5、控制端口，
 *         6、dmduID，
 *         7、报警ID，
 *         8、云台地址，
 *         9、控制卡类型名称
 *
 *返回-1表示读取错误，返回0表示读取了空数据，返回1表示读取成功.
 */
int deploy_all(ALLINF **inf);
/*
 *释放ALLINF结构的空间
 */
int free_all(ALLINF **inf);

#endif
