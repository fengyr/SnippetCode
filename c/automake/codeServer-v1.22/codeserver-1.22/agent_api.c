#include "agent_api.h"
#include "my_errno.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>


const char* trap_msg[] = {
     "FATAL",           /* 导致程序异常退出的错误 */
     "ERROR",           /* 重大错误，导致系统不能正常运行 */
     "WARN",            /* 对系统有影响的错误信息 */
     "INFO",            /* 一般提示，如模块启动等 */
     "DEBUG",           /* 调试信息，如某个变量的值等 */
     "IO"               /* 输入输出类的信息，如程序的IO */
};

const char* oid_type_msg[] = {
     "integer",         /* 整型 */
     "ulong",           /* 无符号长整型 */
     "float",           /* 浮点类型 */
     "string",          /* 字符串类型 */
     "bool",            /* 布尔类型 */
     "ronly",           /* 只读权限 */
     "arw"              /* 可读写 */
};

AGENT_INF      g_agent = {0};
int            l_agent2mod;
TRAP_LEV       level;

/*
 *主动建立TCP连接
 */
void init_link_tcp(void)
{

     struct sockaddr_in client;
     int port = AGENT_PORT_MOD;
     int lport = 0;
     pid_t pid;
     char buf[20];
     socklen_t t;

     if(g_agent.fd != 0){
          return;
     }
     if((g_agent.lfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
          my_perror("Creating socket failed.");
          exit(1);
     }

     memset(&client,0,sizeof(client));
     client.sin_family = AF_INET;
     client.sin_port = htons(port);
     inet_aton("127.0.0.1",&(client.sin_addr));
     if(connect(g_agent.lfd,
		(struct sockaddr *)&client,
		sizeof(struct sockaddr)) < 0){
       my_perror("link error");
       exit(1);
     }

     if((g_agent.fd = socket(AF_INET,SOCK_STREAM,0)) == -1){
       my_perror("Creating socket failed.");
       exit(1);
     }
     memset(&client,0,sizeof(client));
     client.sin_family = AF_INET;
     inet_aton("127.0.0.1",&(client.sin_addr));
     
     if (bind(g_agent.fd, (struct sockaddr *)&client, sizeof(struct sockaddr))<0){
       my_perror("Bind local socket failed.");
       exit(1);
     }
     t=sizeof(struct sockaddr);
     getsockname(g_agent.fd, (struct sockaddr *)&client, &t);
     lport=ntohs(client.sin_port);
     pid = getpid();

     sprintf(buf, "pid:%d\033|port:%d\033|OID:%s\033|", pid, lport, getenv("OID"));
     write(g_agent.lfd, buf, strlen(buf));
     return;
}

/*
 *发送本程序的trap信息
 *返回发送字节数
 */
int snmptrap(TRAP_LEV level, char *msg)
{
     init_link_tcp();
     int n;
     char buf[2048];
     sprintf(buf, "trap::%s:%s\033|",trap_msg[(int)level], msg);
     n = write(g_agent.lfd, buf, strlen(buf));
     return n;
}


/*
 *发送本程序OID的trap信息
 *返回发送字节数
 */
int snmptrap_oid(TRAP_LEV level, oid *trap_oid, size_t oid_len, char *msg, oid *trapbindoid, size_t bindnum)
{
     init_link_tcp();
     int n, i, len, ll;
     oid bind[64];
     oid *find_oid;
     char buf[2048], oid_chr[256];
     int number;
     char temp[2048];
     oid_chr[0] = '\0';
     for(n = 0; n < oid_len; n++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", trap_oid[n]);
          strcat(oid_chr, buf);
     }
     sprintf(buf, "trap:%s:%s:%s", oid_chr, trap_msg[(int)level], msg);
     if(trapbindoid != NULL && bindnum != 0){
          len = 0;
          int flag, m;
          for(n = 0; n < bindnum; n++){
               for(ll = len; trapbindoid[ll] != -1; ll++){
                    bind[ll-len] = trapbindoid[ll];
               }
               flag = len;
               len = ll + 1;
               ll = ll - flag;
               find_oid = malloc(ll * sizeof(oid));
               for(i = 0; i < ll; i++){
                    find_oid[i] = bind[i];
               }
               number = find(find_oid, ll);
               if(number != -1){
                    strcat(buf, ":");
                    strcat(buf, g_agent.mod_regist_oid.regist_oid_chr[number]);
                    strcat(buf, ":");
                    switch(g_agent.mod_regist_oid.regist_oid_type[number][0]){
                    case oid_type_int:
                         strcat(buf, "i:");
                         m = strlen(buf);
                         sprintf(temp, "%d", *(int *)g_agent.mod_regist_oid.value[number]);
                         for(i = m; temp[i-m]; i++){
                              buf[i] = temp[i-m];
                         }
                         buf[i] = 0;
                         break;
                    case oid_type_float:
                         strcat(buf, "f:");
                         m = strlen(buf);
                         sprintf(temp, "%10f", *(float *)g_agent.mod_regist_oid.value[number]);
                         for(i = m; temp[i-m]; i++){
                              buf[i] = temp[i-m];
                         }
                         buf[i] = 0;
                         break;
                    case oid_type_ulong:
                         strcat(buf, "u:");
                         m = strlen(buf);
                         sprintf(temp, "%ld", *(u_long *)g_agent.mod_regist_oid.value[number]);
                         for(i = m; temp[i-m]; i++){
                              buf[i] = temp[i-m];
                         }
                         buf[i] = 0;
                         break;
                    case oid_type_string:
                         strcat(buf, "s:");
                         m = strlen(buf);
                         for(i = m; ((char *)g_agent.mod_regist_oid.value[number])[i-m]; i++){
                              buf[i] = ((char *)g_agent.mod_regist_oid.value[number])[i-m];
                         }
                         buf[i] = 0;
                         break;
                    case oid_type_bool:
                         strcat(buf, "b:");
                         if(*(Bool *)g_agent.mod_regist_oid.value[number] == False){
                              strcat(buf, "False");
                         }
                         else if(*(Bool *)g_agent.mod_regist_oid.value[number] == True){
                              strcat(buf, "True");
                         }
                         break;
		    case oid_access_ronly:
		    case oid_access_rdwr:
		      break;	/* FIXME */
                    }
               }
          }
     }
     strcat(buf, "\033|");
     n = write(g_agent.lfd, buf, strlen(buf));
     return n;
}

/*
 *查找该OID
 *找到返回序号
 *否则返回-1
 */
int find(oid *find_oid, size_t oid_len)
{
     int i, j;
     for(i = 0; i < g_agent.mod_regist_oid.regist_oid_quantity; i++){
          if(oid_len != g_agent.mod_regist_oid.regist_oid_len[i]){
               continue;
          }
          for(j = 0; j < oid_len; j++){
               if(g_agent.mod_regist_oid.regist_oid[i][j]
                  != find_oid[j]){
                    break;
               }
          }
          if(j == oid_len){
               return i;
          }
     }
     return -1;
}
/*
 *发送注册信息
 */
void register_agent(OID_TYPE type, OID_TYPE access, char *ord_chr)
{
     init_link_tcp();
//     usleep(20000);
     char buf[256];
     sprintf(buf, "regist:%c:%c:%s\033|",
             oid_type_msg[(int)type][0],
             oid_type_msg[(int)access][0],
             ord_chr);
     write(g_agent.lfd, buf, strlen(buf));
     return;
}

/*
 *register_read_only_int_instance
 *注册整型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_read_only_int_instance(oid *reg_oid, size_t reg_oid_len,
                                    int *it, void *call)
{
     if(find(reg_oid, reg_oid_len) != -1){
          errno = SNMP_OID_REPEAT;
          return -1;
     }
     int i;
     char oid_chr[256], buf[32];
     oid_chr[0] = '\0';
     g_agent.mod_regist_oid.regist_oid_len[g_agent.mod_regist_oid.regist_oid_quantity]
          = reg_oid_len;
     g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(reg_oid_len * sizeof(oid));
     if(g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
        == NULL){
          return -1;
     }
     for(i = 0; i < reg_oid_len; i++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", reg_oid[i]);
          strcat(oid_chr, buf);
          g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity][i]
               = reg_oid[i];
     }
     g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(strlen(oid_chr)+1);
     strcpy(g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity], oid_chr);
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][0]
          = oid_type_int;
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][1]
          = oid_access_ronly;
     g_agent.mod_regist_oid.value[g_agent.mod_regist_oid.regist_oid_quantity]
          = it;
     if(call != NULL){
          g_agent.mod_regist_oid.oid_call[g_agent.mod_regist_oid.regist_oid_quantity]
               = call;
     }
     register_agent(oid_type_int, oid_access_ronly, oid_chr);
     g_agent.mod_regist_oid.regist_oid_quantity++;
     return 0;
}

/*
 *register_int_instance
 *注册整型OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_int_instance(oid *reg_oid, size_t reg_oid_len,
                          int *it, void *call)
{
     if(find(reg_oid, reg_oid_len) != -1){
          errno = SNMP_OID_REPEAT;
          return -1;
     }
     int i;
     char oid_chr[256], buf[32];
     oid_chr[0] = '\0';
     g_agent.mod_regist_oid.regist_oid_len[g_agent.mod_regist_oid.regist_oid_quantity]
          = reg_oid_len;
     g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(reg_oid_len * sizeof(oid));
     if(g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
        == NULL){
          return -1;
     }
     for(i = 0; i < reg_oid_len; i++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", reg_oid[i]);
          strcat(oid_chr, buf);
          g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity][i]
               = reg_oid[i];
     }
     g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(strlen(oid_chr)+1);
     strcpy(g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity], oid_chr);
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][0]
          = oid_type_int;
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][1]
          = oid_access_rdwr;
     g_agent.mod_regist_oid.value[g_agent.mod_regist_oid.regist_oid_quantity]
          = it;
     if(call != NULL){
          g_agent.mod_regist_oid.oid_call[g_agent.mod_regist_oid.regist_oid_quantity]
               = call;
     }
     register_agent(oid_type_int, oid_access_rdwr, oid_chr);
     g_agent.mod_regist_oid.regist_oid_quantity++;
     return 0;
}

/*
 *register_read_only_ulong_instance
 *注册无符号长整型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_read_only_ulong_instance(oid *reg_oid, size_t reg_oid_len,
                          u_long *it, void *call)
{
     if(find(reg_oid, reg_oid_len) != -1){
          errno = SNMP_OID_REPEAT;
          return -1;
     }
     int i;
     char oid_chr[256], buf[32];
     oid_chr[0] = '\0';
     g_agent.mod_regist_oid.regist_oid_len[g_agent.mod_regist_oid.regist_oid_quantity]
          = reg_oid_len;
     g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(reg_oid_len * sizeof(oid));
     if(g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
        == NULL){
          return -1;
     }
     for(i = 0; i < reg_oid_len; i++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", reg_oid[i]);
          strcat(oid_chr, buf);
          g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity][i]
               = reg_oid[i];
     }
     g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(strlen(oid_chr)+1);
     strcpy(g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity], oid_chr);
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][0]
          = oid_type_ulong;
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][1]
          = oid_access_ronly;
     g_agent.mod_regist_oid.value[g_agent.mod_regist_oid.regist_oid_quantity]
          = it;
     if(call != NULL){
          g_agent.mod_regist_oid.oid_call[g_agent.mod_regist_oid.regist_oid_quantity]
               = call;
     }
     register_agent(oid_type_ulong, oid_access_rdwr, oid_chr);
     g_agent.mod_regist_oid.regist_oid_quantity++;
     return 0;
}

/*
 *register_ulong_instance
 *注册无符号长整型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_ulong_instance(oid *reg_oid, size_t reg_oid_len,
                            u_long *it, void *call)
{
     if(find(reg_oid, reg_oid_len) != -1){
          errno = SNMP_OID_REPEAT;
          return -1;
     }
     int i;
     char oid_chr[256], buf[32];
     oid_chr[0] = '\0';
     g_agent.mod_regist_oid.regist_oid_len[g_agent.mod_regist_oid.regist_oid_quantity]
          = reg_oid_len;
     g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(reg_oid_len * sizeof(oid));
     if(g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
        == NULL){
          return -1;
     }
     for(i = 0; i < reg_oid_len; i++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", reg_oid[i]);
          strcat(oid_chr, buf);
          g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity][i]
               = reg_oid[i];
     }
     g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(strlen(oid_chr)+1);
     strcpy(g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity], oid_chr);
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][0]
          = oid_type_ulong;
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][1]
          = oid_access_rdwr;
     g_agent.mod_regist_oid.value[g_agent.mod_regist_oid.regist_oid_quantity]
          = it;
     if(call != NULL){
          g_agent.mod_regist_oid.oid_call[g_agent.mod_regist_oid.regist_oid_quantity]
               = call;
     }
     register_agent(oid_type_ulong, oid_access_rdwr, oid_chr);
     g_agent.mod_regist_oid.regist_oid_quantity++;
     return 0;
}

/*
 *register_read_only_float_instance
 *注册浮点型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_read_only_float_instance(oid *reg_oid, size_t reg_oid_len,
                                      float *it, void *call)
{
     if(find(reg_oid, reg_oid_len) != -1){
          errno = SNMP_OID_REPEAT;
          return -1;
     }
     int i;
     char oid_chr[256], buf[32];
     oid_chr[0] = '\0';
     g_agent.mod_regist_oid.regist_oid_len[g_agent.mod_regist_oid.regist_oid_quantity]
          = reg_oid_len;
     g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(reg_oid_len * sizeof(oid));
     if(g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
        == NULL){
          return -1;
     }
     for(i = 0; i < reg_oid_len; i++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", reg_oid[i]);
          strcat(oid_chr, buf);
          g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity][i]
               = reg_oid[i];
     }
     g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(strlen(oid_chr)+1);
     strcpy(g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity], oid_chr);
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][0]
          = oid_type_float;
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][1]
          = oid_access_ronly;
     g_agent.mod_regist_oid.value[g_agent.mod_regist_oid.regist_oid_quantity]
          = it;
     if(call != NULL){
          g_agent.mod_regist_oid.oid_call[g_agent.mod_regist_oid.regist_oid_quantity]
               = call;
     }
     register_agent(oid_type_float, oid_access_ronly, oid_chr);
     g_agent.mod_regist_oid.regist_oid_quantity++;
     return 0;
}

/*
 *register_float_instance
 *注册浮点型OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_float_instance(oid *reg_oid, size_t reg_oid_len,
                            float *it, void *call)
{
     if(find(reg_oid, reg_oid_len) != -1){
          errno = SNMP_OID_REPEAT;
          return -1;
     }
     int i;
     char oid_chr[256], buf[32];
     oid_chr[0] = '\0';
     g_agent.mod_regist_oid.regist_oid_len[g_agent.mod_regist_oid.regist_oid_quantity]
          = reg_oid_len;
     g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(reg_oid_len * sizeof(oid));
     if(g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
        == NULL){
          return -1;
     }
     for(i = 0; i < reg_oid_len; i++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", reg_oid[i]);
          strcat(oid_chr, buf);
          g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity][i]
               = reg_oid[i];
     }
     g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(strlen(oid_chr)+1);
     strcpy(g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity], oid_chr);
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][0]
          = oid_type_float;
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][1]
          = oid_access_rdwr;
     g_agent.mod_regist_oid.value[g_agent.mod_regist_oid.regist_oid_quantity]
          = it;
     if(call != NULL){
          g_agent.mod_regist_oid.oid_call[g_agent.mod_regist_oid.regist_oid_quantity]
               = call;
     }
     register_agent(oid_type_float, oid_access_rdwr, oid_chr);
     g_agent.mod_regist_oid.regist_oid_quantity++;
     return 0;
}

/*
 *register_read_only_string_instance
 *注册字符串型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_read_only_string_instance(oid *reg_oid, size_t reg_oid_len,
                                       char *it, void *call)
{
     if(find(reg_oid, reg_oid_len) != -1){
          errno = SNMP_OID_REPEAT;
          return -1;
     }
     int i;
     char oid_chr[256], buf[32];
     oid_chr[0] = '\0';
     g_agent.mod_regist_oid.regist_oid_len[g_agent.mod_regist_oid.regist_oid_quantity]
          = reg_oid_len;
     g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(reg_oid_len * sizeof(oid));
     if(g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
        == NULL){
          return -1;
     }
     for(i = 0; i < reg_oid_len; i++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", reg_oid[i]);
          strcat(oid_chr, buf);
          g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity][i]
               = reg_oid[i];
     }
     g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(strlen(oid_chr)+1);
     strcpy(g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity], oid_chr);
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][0]
          = oid_type_string;
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][1]
          = oid_access_ronly;
     g_agent.mod_regist_oid.value[g_agent.mod_regist_oid.regist_oid_quantity]
          = it;
     if(call != NULL){
          g_agent.mod_regist_oid.oid_call[g_agent.mod_regist_oid.regist_oid_quantity]
               = call;
     }
     register_agent(oid_type_string, oid_access_ronly, oid_chr);
     g_agent.mod_regist_oid.regist_oid_quantity++;
     return 0;
}

/*
 *register_string_instance
 *注册字符串型OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_string_instance(oid *reg_oid, size_t reg_oid_len,
                             char *it, void *call)
{
     if(find(reg_oid, reg_oid_len) != -1){
          errno = SNMP_OID_REPEAT;
          return -1;
     }
     int i;
     char oid_chr[256], buf[32];
     oid_chr[0] = '\0';
     g_agent.mod_regist_oid.regist_oid_len[g_agent.mod_regist_oid.regist_oid_quantity]
          = reg_oid_len;
     g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(reg_oid_len * sizeof(oid));
     if(g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
        == NULL){
          return -1;
     }
     for(i = 0; i < reg_oid_len; i++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", reg_oid[i]);
          strcat(oid_chr, buf);
          g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity][i]
               = reg_oid[i];
     }
     g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(strlen(oid_chr)+1);
     strcpy(g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity], oid_chr);
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][0]
          = oid_type_string;
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][1]
          = oid_access_rdwr;
     g_agent.mod_regist_oid.value[g_agent.mod_regist_oid.regist_oid_quantity]
          = it;
     if(call != NULL){
          g_agent.mod_regist_oid.oid_call[g_agent.mod_regist_oid.regist_oid_quantity]
               = call;
     }
     register_agent(oid_type_string, oid_access_rdwr, oid_chr);
     g_agent.mod_regist_oid.regist_oid_quantity++;
     return 0;
}

/*
 *register_read_only_bool_instance
 *注册布尔型只读OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_read_only_bool_instance(oid *reg_oid, size_t reg_oid_len,
                                     Bool *it, void *call)
{
     if(find(reg_oid, reg_oid_len) != -1){
          errno = SNMP_OID_REPEAT;
          return -1;
     }
     int i;
     char oid_chr[256], buf[32];
     oid_chr[0] = '\0';
     g_agent.mod_regist_oid.regist_oid_len[g_agent.mod_regist_oid.regist_oid_quantity]
          = reg_oid_len;
     g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(reg_oid_len * sizeof(oid));
     if(g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
        == NULL){
          return -1;
     }
     for(i = 0; i < reg_oid_len; i++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", reg_oid[i]);
          strcat(oid_chr, buf);
          g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity][i]
               = reg_oid[i];
     }
     g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(strlen(oid_chr)+1);
     strcpy(g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity], oid_chr);
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][0]
          = oid_type_bool;
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][1]
          = oid_access_ronly;
     g_agent.mod_regist_oid.value[g_agent.mod_regist_oid.regist_oid_quantity]
          = it;
     if(call != NULL){
          g_agent.mod_regist_oid.oid_call[g_agent.mod_regist_oid.regist_oid_quantity]
               = call;
     }
     register_agent(oid_type_bool, oid_access_ronly, oid_chr);
     g_agent.mod_regist_oid.regist_oid_quantity++;
     return 0;
}

/*
 *register_bool_instance
 *注册布尔型OID
 *注册失败返回-1
 *重复为SNMP_OID_REPEAT
 *成功返回0
 */
int register_bool_instance(oid *reg_oid, size_t reg_oid_len,
                           Bool *it, void *call)
{
     if(find(reg_oid, reg_oid_len) != -1){
          errno = SNMP_OID_REPEAT;
          return -1;
     }
     int i;
     char oid_chr[256], buf[32];
     oid_chr[0] = '\0';
     g_agent.mod_regist_oid.regist_oid_len[g_agent.mod_regist_oid.regist_oid_quantity]
          = reg_oid_len;
     g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(reg_oid_len * sizeof(oid));
     if(g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity]
        == NULL){
          return -1;
     }
     for(i = 0; i < reg_oid_len; i++){
          strcat(oid_chr, ".");
          sprintf(buf, "%d", reg_oid[i]);
          strcat(oid_chr, buf);
          g_agent.mod_regist_oid.regist_oid[g_agent.mod_regist_oid.regist_oid_quantity][i]
               = reg_oid[i];
     }
     g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity]
          = malloc(strlen(oid_chr)+1);
     strcpy(g_agent.mod_regist_oid.regist_oid_chr[g_agent.mod_regist_oid.regist_oid_quantity], oid_chr);
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][0]
          = oid_type_bool;
     g_agent.mod_regist_oid.regist_oid_type[g_agent.mod_regist_oid.regist_oid_quantity][1]
          = oid_access_rdwr;
     g_agent.mod_regist_oid.value[g_agent.mod_regist_oid.regist_oid_quantity]
          = it;
     if(call != NULL){
          g_agent.mod_regist_oid.oid_call[g_agent.mod_regist_oid.regist_oid_quantity]
               = call;
     }
     register_agent(oid_type_bool, oid_access_rdwr, oid_chr);
     g_agent.mod_regist_oid.regist_oid_quantity++;
     return 0;
}


/*
 *snmpset()
 *修改OID的值
 */
void agent_snmpset(int number, char *value)
{
     if(g_agent.mod_regist_oid.oid_call[number] != NULL){
          
		switch((int)g_agent.mod_regist_oid.regist_oid_type[number][0]){
		case (int)oid_type_int:
		{
			int val;
			val = atoi(value);
			*(int *)(g_agent.mod_regist_oid.value[number]) = val;
			break;
		}
		case (int)oid_type_ulong:
		{
			u_long val;
			val = (u_long)(atol(value));
			*(u_long *)(g_agent.mod_regist_oid.value[number]) = val;
			break;
		}
		case (int)oid_type_float:
		{
			float val;
			val = atof(value);
			*(float *)(g_agent.mod_regist_oid.value[number]) = val;
			break;
		}
		case (int)oid_type_string:
		{
			strcpy((char *)g_agent.mod_regist_oid.value[number], value);
			break;
		}
		case (int)oid_type_bool:
		{
			Bool val;
			if(memcmp(value, "true", 4) == 0){
				val = True;
			}
			else{
				val = False;
			}
			*(Bool *)(g_agent.mod_regist_oid.value[number]) = val;
			break;
		}
		default:return;
		}
	 
		g_agent.mod_regist_oid.oid_call[number](snmpset);
	}
     return;
}

/*
 *agent_set()
 *接受网管的修改OID值的请求
 */
void agent_set(char *buf, int fd)
{
     oid set_oid[256] = {0};
     size_t oid_len = 0;
     int i,j, number;
     for(i = 0; buf[i] != ':'; i++){
          if(buf[i] == '.'){
               oid_len++;
          }
     }
     for(i = 0, j = 0; buf[i] != ':'; i++){
          if(buf[i] == '.'){
               set_oid[j] = atoi(&buf[i+1]);
               j++;
          }
     }
     number = find(set_oid, oid_len);
     if(number == -1){
          write(fd, "set:fail\033|", 10);
          return;
     }
     agent_snmpset(number, &buf[i+1]);
     write(fd, "set:true\033|", 10);
     return;
}

/*
 *agent_get()
 *获取OID的值
 */
void agent_get(char *getbuf, int fd)
{
     oid get_oid[256] = {0};
     size_t oid_len = 0;
     int i,j, number,l;
     char buf[1024];
     char num_flag[1024];
     
     for(i = 0; getbuf[i]; i++){
          if(getbuf[i] == '.'){
               oid_len++;
          }
     }
     for(i = 0, j = 0; getbuf[i] != ':' && getbuf[i] != 0; i++){
          if(getbuf[i] == '.'){
	       for(l = i+1; getbuf[l] >= '0' && getbuf[l] <= '9'; l++){
		       num_flag[l - i -1] = getbuf[l];
	       }
	       num_flag[l - i -1] = 0;
               get_oid[j] = atoi(num_flag);
               j++;
          }
     }
     number = find(get_oid, oid_len);
     if(g_agent.mod_regist_oid.oid_call[number] != NULL){
       g_agent.mod_regist_oid.oid_call[number](snmpget);
     }
     switch((int)g_agent.mod_regist_oid.regist_oid_type[number][0]){
     case (int)oid_type_int:
          sprintf(buf, "get:%d", *(int *)(g_agent.mod_regist_oid.value[number]));
          break;
     case (int)oid_type_ulong:
          sprintf(buf, "get:%ld", *(u_long *)(g_agent.mod_regist_oid.value[number]));
          break;
     case (int)oid_type_float:
          sprintf(buf, "get:%10f", *(float *)(g_agent.mod_regist_oid.value[number]));
          break;
     case (int)oid_type_string:
          sprintf(buf, "get:%s", (char *)g_agent.mod_regist_oid.value[number]);
          break;
     case (int)oid_type_bool:
          if(*(Bool *)(g_agent.mod_regist_oid.value[number]) == True){
               sprintf(buf, "get:true");
          }
          else{
               sprintf(buf, "get:false");
          }
          break;
     }
     int nn;
     nn = strlen(buf);
     buf[nn + 2] = '\0';
     buf[nn+1] = '|';
     buf[nn] = '\033';
     write(fd, buf, nn+2);
     return;
}


/*
 *一次读取一条指令
 *返回number表示长度
 *0表示连接中断
 *-1错误
 */
int tcprecv(int fd, char *buf, size_t len)
{
     int i, n,flag = 0;
     for(i = 0; ; i++){
          n = read(fd, &buf[i], 1);
          if(n < 0){
               my_perror("read");
               return -1;
          }
          if(n == 0){
               return 0;
          }
          if(buf[i] == '\033'){
               flag = 1;
               continue;
          }
          if(buf[i] == '|' && flag == 1){
               buf[i] = '\0';
               buf[i-1] = '\0';
               return (i-1);
          }
          else if(flag == 1){
               flag = 0;
          }
     }
}


//#ifdef THREAD_ON

/*
 *snmp_init()
 *初始化snmp
 */
void snmp_thread(void)
{
     init_link_tcp();
     pthread_t thread;
     if(pthread_create(&thread, NULL, (void*)snmp_server, NULL)){
          perror("Pthread_create() error.");
          return;
     }
}

void  snmp_server(void)
{
     int n;
     char buf[1024];
     int fd;
     struct sockaddr_in addr;
     size_t t;
//     while((n = read(g_agent.fd, buf, 1024))){
     listen(g_agent.fd, 5);
     while(1){
       t=sizeof(struct sockaddr);
       fd=accept(g_agent.fd, (struct sockaddr *)&addr, &t);
       while (1){
	 n=tcprecv(fd, buf, 1024);
	 if (n<=0) break;
	 //          printf("2 %d\n", n);
	 buf[n] = '\0';
	 while(buf[n-1] == '\n' || buf[n-1] == '\r'){
	   n--;
	   buf[n] = '\0';
	 }
	 //          printf("%s\n",buf);
	 if(memcmp(buf, "set", 3) == 0){
	   agent_set(&buf[4], fd);
	 }
	 else if(memcmp(buf, "get", 3) == 0){
	   agent_get(&buf[4], fd);
	 }
       }
     }
     exit(1);
     pthread_exit(NULL);
}

//#endif

#if 0
/*
 *snmp_init()
 *初始化snmp
 */
void snmp_poll(int sec)
{
     init_link_tcp();
     fd_set allset;
     int max;
     FD_ZERO(&allset);
     FD_SET(g_agent.fd, &allset);
     max = g_agent.fd + 1;
     struct timeval timeout;
     timeout.tv_sec = sec;
     timeout.tv_usec = 0;
     if(select(max, &allset, NULL, NULL, &timeout)){
          if(FD_ISSET(g_agent.fd, &allset)){
               int n;
               char buf[1024];
//               n = read(g_agent.fd, buf, 1024);
               n= tcprecv(g_agent.fd, buf, 1024);
               if(n>0){
                    buf[n] = '\0';
                    while(buf[n-1] == '\n' || buf[n-1] == '\r'){
                         n--;
                         buf[n] = '\0';
                    }
                    if(memcmp(buf, "set", 3) == 0){
                         agent_set(&buf[4]);
                    }
                    else if(memcmp(buf, "get", 3) == 0){
                         agent_get(&buf[4]);
                    }
               }
               else{
                    exit(1);
               }
          }
     }
     return;
}

#endif
