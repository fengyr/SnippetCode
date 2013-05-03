#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
//extern IsDetectCollapse;
extern int IsDetectCollapse;
extern int IntoWBMode;  //增加变成0的过程
extern int WBDataType;
extern int TrainType, TrainSample, TrainMode; //训练的类数，以及每类的数量
extern float colorDis;
extern float sepangle;
extern float collapse_thresh;
extern Rect WBRect; //x, y, width, heigh
extern int WBDataType;
extern int FrameCounter;

struct cmd
{
    int cmdID;
    char msg[100];
};

void *RecvInterface (void *a)
{
    int serverSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    // 用port保存使用的端口
    int port = 11014;

    // 建立Socket，并设置
    serverSocket = socket (AF_INET, SOCK_STREAM, 0);

    // 设置socket选项，这是可选的，可以避免服务器程序结束后无法快速重新运行
    int val = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof (val));

    // 定义端口和监听的地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons (port);
    serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
    memset(&(serverAddr.sin_zero), 0, 8);
    int rc = bind(serverSocket, (struct sockaddr *) &serverAddr,
                   sizeof (struct sockaddr));
    if (rc == -1)
    {
        printf ("Bad bind\n");
        exit (1);
    }

    // 让serverSocket开始监听，客户队列长为5
    rc = listen(serverSocket, 5);
    if (rc == -1)
    {
        printf ("Bad listen\n");
        exit (1);
    }

    while (1)
    {
        // 等待客户连接
        int clientsock;
        int clientAddrSize = sizeof(struct sockaddr_in);
        clientsock = accept(serverSocket,
                            (struct sockaddr *) &clientAddr,
                            (socklen_t *) & clientAddrSize);
        //接收
        cmd cmd_ui;
        //memset(receiveBuf,0,sizeof(receiveBuf));
        //接收客户的消息
        recv (clientsock, &cmd_ui, sizeof(cmd_ui) + 1, 0);
        printf ("%s\n", cmd_ui.msg);
        printf ("%d\n", cmd_ui.cmdID);
        printf("start recieve\n");

        if (cmd_ui.cmdID == 1)
            //色号模板数
        {
            if (cmd_ui.msg == "")
            {
                printf ("色号模板数错误\n");
            }
            else
            {
                sscanf(cmd_ui.msg,"%d %d",&TrainType,&TrainSample);
                //TrainType = atoi (cmd_ui.msg);
                TrainMode = TrainType;
                FrameCounter = 0;
            }
            printf ("TrainType %d\n", TrainType);
        }
        if (cmd_ui.cmdID == 2)
            //是否检测崩边崩角
        {
            if (atoi(cmd_ui.msg) == 1)
            {
                IsDetectCollapse = 1;
            }
            if(atoi(cmd_ui.msg) == 0)
            {
                IsDetectCollapse = 0;
            }
            printf ("********************IsDetectCollapse %d\n", IsDetectCollapse);
        }

        if (cmd_ui.cmdID == 3)
            //是否进入调试模式
        {
            printf("~~~~~~~~~~~~~~~~into wbmode %d\n",atoi(cmd_ui.msg));
            char temp[40];
            if (cmd_ui.msg == "")
            {
                printf ("调试模式错误\n");
            }
            if (atoi(cmd_ui.msg) == 1)
            {
                IntoWBMode = 1;
            }
            if(atoi(cmd_ui.msg) == 0)
            {
                IntoWBMode = 0;
            }
            printf("IntoWBMode %d\n",IntoWBMode);
        }

        if (cmd_ui.cmdID == 4)
            //设置色差阈值
        {
            if (cmd_ui.msg == "")
            {
                printf ("色差阈值错误\n");
            }
            else
            {
                colorDis = atof (cmd_ui.msg);
            }
            printf ("colorDis %d\n", colorDis);
        }

        if (cmd_ui.cmdID == 5)
            //设置崩边阈值
        {
            if (cmd_ui.msg == "")
            {
                printf ("崩边阈值错误\n");
            }
            else
            {
                collapse_thresh = atof (cmd_ui.msg);
            }
            printf ("collapse %f\n", collapse_thresh);
        }
        if (cmd_ui.cmdID == 6)
            //设置矩形参数
        {
            int x,y,w,h;
            if (cmd_ui.msg == "")
            {
                printf ("矩形数据错误\n");
            }
            else
            {
                sscanf(cmd_ui.msg,"%d %d %d %d %d",&WBDataType,&x,&y,&w,&h);
                WBRect.x=x;
                WBRect.y=y;
                WBRect.width=w;
                WBRect.height=h;
            }
            printf("rect %d %d %d %d\n",x,y,w,h);
            //printf ("collapse %f\n", collapse_thresh);
        }

    }				//recv(clientsock,char * buf,int maxlen,0);
    return NULL;
}
