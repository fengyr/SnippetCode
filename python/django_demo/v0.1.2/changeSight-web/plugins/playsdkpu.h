

#ifdef __cplusplus
	extern "C"{
#endif

#define SOURCE_BUF_MAX 1024 * 1000
#define SOURCE_BUF_MIN 1024 * 50

#define T_AUDIO16	0
#define T_YV12		3

//stream type
#define STREAM_REALTIME	0
#define STREAM_FILE	1

typedef struct tagRect{
	int x;
	int y;
	int uWidth;
	int uHeight;
}PLAYRECT;


typedef struct tagInitInfo{
	int	uWidth;
	int     uHeight;	
}INITINFO;

typedef struct frameinfo{
	long width;
	long height;
	long stamp;
	long type;
	long frame_rate;
}FRAME_INFO;

//ERRORCODE
#define M4PErr_NoError			0x00000000

#define M4PErr_SDLInitFail		0x00000001
#define M4PErr_SDLSetVideoFail		0x00000002
#define M4PErr_CreateYUVOverlayFail	0x00000003
#define M4PErr_SDLAddTimerFail		0x00000004
#define M4PErr_CreateMutexFail		0x00000005

#define M4PErr_InvalidFuction		0x00010000	//add at 2002.8.23
#define M4PErr_InvalidParam		0x00010001
#define M4PErr_M4SDKUnInit		0x00010002
#define M4PErr_OpenFileFail		0x00010003
#define M4PErr_OrderFail		0x00010004
#define M4PErr_AllocMemFail		0x00010005
#define M4PErr_ThreadCreateFail		0x00010006
#define M4PErr_BadFileFormat		0x00010007
#define M4PErr_InitVideoDecoderFail	0x00010008
#define M4PErr_InitAudioDecoderFail	0x00010009

#define M4PErr_DecoderVideoFail		0x0001000a
#define M4PErr_DecoderAudioFail		0x0001000b

#define M4PErr_AudioCreateFail		0x0001000c

#define M4PErr_SurportFileOnly		0x0001000d
#define M4PErr_SurportStreamOnly	0x0001000e
#define M4PErr_BadFileHead		0x0001000f
#define M4PErr_VersionIncorrect		0x00010010
#define M4PErr_BufOverflow		0x00010011

int Hik_PlayM4_Init(INITINFO InitInfo, int using_SDL);	//must be done first
int Hik_PlayM4_DeInit(void);	//must be done last
int Hik_PlayM4_SetDisplay(int width, int height);

int Hik_PlayM4_OpenFile(int nPort,char * sFileName);
int Hik_PlayM4_CloseFile(int nPort);
int Hik_PlayM4_Play(int nPort, PLAYRECT rect);
int Hik_PlayM4_Stop(int nPort);
int Hik_PlayM4_Pause(int nPort,int nPause);
int Hik_PlayM4_Fast(int nPort);
int Hik_PlayM4_Slow(int nPort);

int Hik_PlayM4_SetPlayPos(int nPort, float percent);
float Hik_PlayM4_GetPlayPos(int nPort);

int Hik_PlayM4_SetFileEndCallBack(int nPort, void (*FileEndCallBack)(int nPort));	//modified at 2003.4.4
int Hik_PlayM4_SetDecCallBack(int nPort, void (*DecCallBack)(int nPort, char *pBuf, int size, FRAME_INFO *pFrameInfo, long reservd1, long reserved2));
int Hik_PlayM4_SetDisplayCallBack(int nPort, void (*DisplayCallBack)(int nPort, char *pBuf, long size,long width, long height, long stamp, long type,long reservd));

int Hik_PlayM4_PlaySound(int nPort);
int Hik_PlayM4_StopSound();
int Hik_PlayM4_SetVolume(int nPort, int volume);

int Hik_PlayM4_SetDisplayFrames(int nPort, int num);

int Hik_PlayM4_OneByOne(int nPort);

int Hik_PlayM4_GetFileTime(int nPort);
int Hik_PlayM4_GetPlayedTime(int nPort);
int Hik_PlayM4_GetFileFrames(int nPort);
int Hik_PlayM4_GetPlayedFrames(int nPort);
int Hik_PlayM4_GetCurrentFrameNum(int nPort);
int Hik_PlayM4_GetCurrentFrameRate(int nPort);
int Hik_PlayM4_GetPictureSize(int nPort, int *pwidth, int *pheight);


int Hik_PlayM4_OpenStream(int  nPort, char *pfileheadbuf, int size, int streambufsize);
int Hik_PlayM4_InputData(int nPort, char *pBuf, int nSize);
int Hik_PlayM4_CloseStream(int nPort);

int Hik_PlayM4_ConvertToBmpFile( char *pBuf, long size,long width, long height, long type,char *filename);

int Hik_PlayM4_GetLastErrorCode();

int Hik_PlayM4_SetPlayRect(int nPort, PLAYRECT rect);

int Hik_PlayM4_GetSourceBufferRemain(int nPort);
int Hik_PlayM4_ResetSourceBuffer(int nPort);

int Hik_PlayM4_ThrowBFrameNum(int nPort, unsigned int num);
int Hik_PlayM4_SetPicQuality(int nPort, unsigned int hightquality);
int Hik_PlayM4_GetPicQuality(int nPort, unsigned int *hightquality);

int Hik_PlayM4_SetFileRefCallBack(int nPort, void (*FileRefCallBack)(int nPort));
int Hik_PlayM4_OneByOneBack(int nPort);
int Hik_PlayM4_SetCurrentFrameNum(int nPort, unsigned int framenum);

//add at 2003.5.16
int Hik_PlayM4_OpenStreamEx(int  nPort, char *pfileheadbuf, int size, int streambufsize);
int Hik_PlayM4_InputVideoData(int nPort, char *pBuf, int nSize);
int Hik_PlayM4_InputAudioData(int nPort, char *pBuf, int nSize);
int Hik_PlayM4_CloseStreamEx(int nPort);

//add at 2003.5.21
int Hik_PlayM4_SetOpenStreamMode(int  nPort, int mode);
int Hik_PlayM4_GetStreamMode(int nPort);

#ifdef __cplusplus
	}
#endif

