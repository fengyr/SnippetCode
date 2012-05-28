#ifndef _AGENT_API_H_
#define _AGENT_API_H_

#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

#include <pthread.h>

//#ifdef THREAD_OFF
#include <sys/time.h>
#include <sys/select.h>
//#endif

#define AGENT_PORT_MOD           42424
#define MOD_REG_OID_MAX          2048

typedef int oid;

typedef enum callback_e
{
     snmpset,
     snmpget
}CALLBACK;

typedef enum trap_lev_e
{
     fatal,
     error,
     warn,
     info,
     debug,
     io
}TRAP_LEV;

typedef enum oid_type_e
{
     oid_type_int,      /* 整型 */
     oid_type_ulong,    /* 无符号长整型 */
     oid_type_float,    /* 浮点类型 */
     oid_type_string,   /* 字符串类型 */
     oid_type_bool,     /* 布尔类型 */
     oid_access_ronly,  /* 权限：只读 */
     oid_access_rdwr    /* 权限：可读写 */
}OID_TYPE;

typedef struct reg_oid_s
{
     int        regist_oid_quantity;                 /* 本进程注册的OID数量 */
     oid       *regist_oid[MOD_REG_OID_MAX];         /* 本进程注册的OID */
     int        regist_oid_len[MOD_REG_OID_MAX];     /* 本进程注册的OID的长度 */
     char      *regist_oid_chr[MOD_REG_OID_MAX];     /* 本进程注册的OID的字符串型 */
     OID_TYPE   regist_oid_type[MOD_REG_OID_MAX][2]; /* 本进程注册的OID的类型 */
     void      *value[MOD_REG_OID_MAX];              /* 对应OID绑定的值 */
     void     (*oid_call[MOD_REG_OID_MAX])(CALLBACK type);
                          /* 对应该OID绑定值改变后调用的call函数 */
}REG_OID;

typedef struct agent_inf_s
{
  int        lfd;                    /* This fd is used to send request to agent server */
  int        fd;                     /* This fd is used to receive order from agent server */
  REG_OID    mod_regist_oid;         /* 注册入模块的OID序列 */
}AGENT_INF;

typedef enum bool_e
{
     False,
     True
}Bool;


/*
 *主动建立TCP连接
 */
void init_link_tcp(void);

/*
 *发送本程序的trap信息
 *返回发送字节数
 */
int snmptrap(TRAP_LEV level, char *msg);
/*
 *发送本程序OID的trap信息
 *返回发送字节数
 */
int snmptrap_oid(TRAP_LEV level, oid *trap_oid, size_t oid_len, char *msg, oid *trapbindoid, size_t bindnum);
/*
 *查找该OID
 *找到返回序号
 *否则返回-1
 */
int find(oid *find_oid, size_t oid_len);
/*
 *发送注册信息
 */
void register_agent(OID_TYPE type, OID_TYPE access, char *ord_chr);
/*
 *register_read_only_int_instance
 *注册整型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_read_only_int_instance(oid *reg_oid, size_t reg_oid_len,
                                    int *it, void *call);
/*
 *register_int_instance
 *注册整型OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_int_instance(oid *reg_oid, size_t reg_oid_len,
                          int *it, void *call);
/*
 *register_read_only_ulong_instance
 *注册无符号长整型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_read_only_ulong_instance(oid *reg_oid, size_t reg_oid_len,
                                      u_long *it, void *call);
/*
 *register_ulong_instance
 *注册无符号长整型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_ulong_instance(oid *reg_oid, size_t reg_oid_len,
                            u_long *it, void *call);
/*
 *register_read_only_float_instance
 *注册浮点型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_read_only_float_instance(oid *reg_oid, size_t reg_oid_len,
                                      float *it, void *call);
/*
 *register_int_instance
 *注册浮点型OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_float_instance(oid *reg_oid, size_t reg_oid_len,
                            float *it, void *call);
/*
 *register_read_only_string_instance
 *注册字符串型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_read_only_string_instance(oid *reg_oid, size_t reg_oid_len,
                                       char *it, void *call);
/*
 *register_string_instance
 *注册字符串型OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_string_instance(oid *reg_oid, size_t reg_oid_len,
                             char *it, void *call);
/*
 *register_read_only_bool_instance
 *注册布尔型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_read_only_bool_instance(oid *reg_oid, size_t reg_oid_len,
                                     Bool *it, void *call);
/*
 *register_bool_instance
 *注册布尔型OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_bool_instance(oid *reg_oid, size_t reg_oid_len,
                           Bool *it, void *call);
/*
 *snmpset()
 *修改OID的值
 */
void agent_snmpset(int number, char *value);
/*
 *agent_set()
 *接受网管的修改OID值的请求
 */
void agent_set(char *buf, int fd);
/*
 *agent_get()
 *获取OID的值
 */
void agent_get(char *getbuf, int fd);
//#ifdef THREAD_ON
/*
 *snmp_init()
 *初始化snmp
 */
void snmp_thread(void);
void snmp_server(void);
//#endif

//#ifdef THREAD_OFF
/*
 *snmp_init()
 *初始化snmp
 */
void snmp_poll(int sec);
//#endif


#endif
