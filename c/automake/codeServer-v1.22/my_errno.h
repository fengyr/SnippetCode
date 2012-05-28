#ifndef _MY_ERRNO_H_
#define _MY_ERRNO_H_

#ifndef _ERRNO_H_
#define _ERRNO_H_
#include <errno.h>
#endif
#ifndef _STDLIB_H_
#define _STDLIB_H_
#include <stdlib.h>
#endif

#define MY_NO_MIN    1000
#define MY_NO_MAX    1010

#define      MY_ERR_NO_ERROR      (MY_NO_MIN+0) /* No error */
#define      MY_ERR_ARG_ERROR     (MY_NO_MIN+1) /* Argument input error */
#define      MY_ERR_CREATFIFO     (MY_NO_MIN+2) /* Could not create fifo */
#define      MY_ERR_OPENFIFO      (MY_NO_MIN+3) /* Could not open fifo */
#define      MY_ERR_NO_DEPLOY     (MY_NO_MIN+4) /* Deploy not exist */
#define      MY_ERR_DEPLOY_FALSE  (MY_NO_MIN+5) /* Deploy false:too long or wrong */
#define      MY_ERR_LIST_EMPTY    (MY_NO_MIN+6) /* The list is empty */
#define      SNMP_OID_REPEAT      (MY_NO_MIN+7) /* The OID is repeat */

char* my_errmsg[] = {
     /* 0 */        "No error",
     /* 1 */        "Argument input error",
     /* 2 */        "Could not create fifo",
     /* 3 */        "Could not open fifo",
     /* 4 */        "Deploy not exist",
     /* 5 */        "Deploy false:too long or wrong",
     /* 6 */        "The list is empty",
     /* 7 */        "The OID is repeat"
};

/**/
void my_perror( char* info)
{
     if ( info ){
          if ( errno >MY_NO_MAX  || errno < MY_NO_MIN ){
               perror(info);
               return;
          }
          else{
               printf("%s: %s\n", info, my_errmsg[errno-MY_NO_MIN] );
               return;
          }
     }
     if ( errno > MY_NO_MAX || errno < MY_NO_MIN ){
          perror(info);
          return;
     }
     else{
          printf("Error: %s\n", my_errmsg[errno-MY_NO_MIN] );
     }
}

#endif
