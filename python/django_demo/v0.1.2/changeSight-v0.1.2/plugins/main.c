#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/shm.h>

#include <sys/time.h> //用于系统计时

#include "SDL.h"


//指向图像的指针
typedef struct{
    long int now;
    char data[1];
}VideoFrame;

VideoFrame* pic=NULL;

int PictureWidth = 704, PictureHeight = 576;
int BufForImage = 6;
SDL_Overlay* overlay=NULL;
SDL_Rect rect;
SDL_Surface* screen=NULL;
char* yDecImage=NULL, *vDecImage=NULL, *uDecImage=NULL;

int Image_handle;




void SDL_Display(Uint8* YFrame, Uint8* UFrame, Uint8* VFrame)
{
    SDL_UnlockSurface(screen);
    overlay->pixels[0] = YFrame;
    overlay->pixels[1] = UFrame;
    overlay->pixels[2] = VFrame;

    SDL_DisplayYUVOverlay(overlay,&rect);
    return;
}


void PicData(VideoFrame* pic)
{
    static int emptytime = 0;
    static long int framegetCount = 2;
    
    printf("The PicData Processing, the Pic->now is %d, framegetCount is %d\n", pic->now, framegetCount);
    while (1)
    {  
	if (framegetCount == pic->now || framegetCount > pic->now)
	{ 
	    emptytime++;
	    
	    usleep(0);      //进程调度时间为 4000us
	    
	    //printf("The Pic->now is %d at empty time %d\n", pic->now, emptytime);
	    //设置系统的等待图像的时间为5000＊400ms
	    if (emptytime == 5000)
	    {
		printf("No Image can be readed! The Program is to Exit\n");
		exit(1);
	    }
	}
	else if(framegetCount < pic->now)
	{
	    int availcontain = 1 + 3;    //允许落后3帧
	    if(pic->now > framegetCount + availcontain)
	    {
		framegetCount = pic->now;
	    }
	    else
	    {
		if(pic->now > framegetCount + 1)
		    ;//printf("Is in the tolerate of frame drop at framegetCount %d\n", framegetCount);
		
		framegetCount ++;
	    }

	    emptytime = 0;
	    break;
	}
    }
    
    int YSize = PictureWidth*PictureHeight;
    int FrameSize = PictureWidth*PictureHeight*3/2;
    

    yDecImage = pic->data + (framegetCount%BufForImage)*FrameSize;
    uDecImage = pic->data + (framegetCount%BufForImage)*FrameSize + YSize;
    vDecImage = pic->data + (framegetCount%BufForImage)*FrameSize + YSize*5/4;
    return;

}

int SDL_init(void)
{
    rect.x=0;
    rect.y=0;
    rect.w = PictureWidth;
    rect.h = PictureHeight;
    
    if ((screen = SDL_SetVideoMode(PictureWidth, PictureHeight, 32,SDL_SWSURFACE)) == NULL){ 
	 printf("deploy.c: SDL_init. SDL 初始化失败!\n");
	 exit(1);
     }
     overlay = SDL_CreateYUVOverlay(PictureWidth, PictureHeight, SDL_IYUV_OVERLAY, screen);
     printf("deploy.c: SDL_init. SDL Ready!\n");
     return 0;
}



char shmName[50] = ".libotest.shm";
void DecImageInit()
{
    
    char createFile[100] = "touch ";
    strcat(createFile, shmName);
    
    FILE *f = popen(createFile, "w");
    pclose(f);

    key_t Image_key=ftok(shmName, 'S');
    if((Image_handle = shmget(Image_key, sizeof(pic->now) + BufForImage*PictureWidth*PictureHeight*3/2, IPC_CREAT | 0666)) < 0)
    {
	perror(" main.c: shmget For Image");
	exit(1);
    }
    
    if(( pic = (VideoFrame*)shmat(Image_handle, 0, 0)) == NULL){
	perror("main.c : shmat For Image");
	exit(1);
    }

    pic->now = 0;
    printf("The DecImageInit is OK\n");
    return;
}




void on_close_file(FILE* f)
{
    char msg[128];
    strcpy(msg,"CloseFile\n");
    fwrite(msg,strlen(msg),1,f);
    fflush(f);

}

void on_close_stream(FILE* f)
{
    char msg[128];
    strcpy(msg,"CloseFile\n");
    fwrite(msg,strlen(msg),1,f);
    fflush(f);
}

void on_plugin_exit(FILE* f)
{
    /* dest decoder plugin exit */
    char msg[128];
    strcpy(msg,"Exit\n");
    fwrite(msg,strlen(msg),1,f);
    fflush(f);
}

void on_open_file(FILE *f,char *filename)
{
    /* open the file */
    char msg[1024] =""; 
        
    sprintf(msg,"OpenFile %s\n",filename);
    fwrite(msg,strlen(msg),1,f);
 
    fflush(f);
    //free(msg);
}

void on_open_stream(FILE *f,char *stream)
{
    /* open the file */
    //char *msg = malloc(1024); 
    char msg[1024] =""; 
    sprintf(msg,"OpenStream MULTICAST:%s\n",stream);
    fwrite(msg,strlen(msg),1,f);
    
    fflush(f);
}


char LocalIP[48];

void getlocalip()
{
    FILE * f;

    f=popen("/sbin/ifconfig | head -2 | tail -1 | cut -d: -f 2 | cut -d\\  -f 1","r");
    fscanf(f,"%s",LocalIP);
    pclose(f);

    return;
}

FILE* on_proc(void)
{
    FILE *f;
    /* start the plugins */
    char *cmd =(char*) malloc(1024);
    //char cmd[1024] = ""; 
    getlocalip();

    //sprintf(cmd, "./dec_hik_dm642_d1 %s %s", shmName, LocalIP);
    sprintf(cmd, "./dec_mpeg2 %s %s", shmName, LocalIP);
    
    f = popen(cmd,"w");
    printf("Cmd is %s\n", cmd);

    free(cmd);
    return f;
}


int main(int argc, char** argv)
{

    DecImageInit();
    SDL_init();
    //开始播放图像
    FILE* decoding = on_proc();
    
    on_open_stream(decoding, argv[1]);
	
    printf("Start to process\n");
    while(1)
    {
	PicData(pic);
	SDL_Display(yDecImage, vDecImage, uDecImage);
    }

}
