/*
File:	ds40xxhcApi.h

Descrpition:
	API interface for DS40XXHC/F

Author:
	Ruihong Wang. writed at 2004.5.26

Last Update: 2005-1-13
*/

#include "DataType.h"
#include <SDL/SDL.h>

#ifdef __cplusplus
        extern "C"{
#endif

typedef void(*STREAM_READ_CALLBACK)(int cahnnelNum, char *pBuf, int frameType,int length);

typedef enum  {
   vdfRGB8A_233              = 0x00000001,
   vdfRGB8R_332              = 0x00000002,
   vdfRGB15Alpha             = 0x00000004,
   vdfRGB16                  = 0x00000008,
   vdfRGB24                  = 0x00000010,
   vdfRGB24Alpha             = 0x00000020,
   
   vdfYUV420Planar           = 0x00000040,
   vdfYUV422Planar           = 0x00000080,
   vdfYUV411Planar           = 0x00000100,
   vdfYUV420Interspersed     = 0x00000200,
   vdfYUV422Interspersed     = 0x00000400,
   vdfYUV411Interspersed     = 0x00000800,
   vdfYUV422Sequence         = 0x00001000,   /* U0, Y0, V0, Y1:  For VO overlay */
   vdfYUV422SequenceAlpha    = 0x00002000,   
   /* U0, Y0, V0, Y1:  For VO overlay, with low bit for alpha blending */
   vdfMono                   = 0x00004000,  /* 8 bit monochrome */

   vdfYUV444Planar           = 0x00008000,
}TypeVideoFormat;


typedef enum {
	brCBR = 0,
	brVBR = 1,
}BitrateControlType_t;

/*
typedef enum {
	DS400XM 			=0,
	DS400XH				=1,
	DS4004HC			=2,
	DS4008HC			=3,
	DS4016HC			=4,
	DS4001HF			=5,
	DS4004HF			=6,
	INVALID_BOARD_TYPE	=0xffffffff,
}BOARD_TYPE_DS;
*/

#define STREAM_TYPE_VIDEO	1
#define STREAM_TYPE_AUDIO	2
#define STREAM_TYPE_AVSYNC	3
#define DRAWFUN(x)				void  (CALLBACK* x)(long nPort,HDC hDc,LONG nUser)

typedef struct tagChannelCapability{
	unsigned char bAudioPreview;
	unsigned char bAlarmIO;
	unsigned char bWatchDog;
}CHANNEL_CAPABILITY, *PCHANNEL_CAPABILITY;

typedef struct tagFramsStatistics{
	unsigned long VideoFrames;
	unsigned long AudioFrames;
	unsigned long FramesLost;
	unsigned long QueueOverflow;
	unsigned long CurBps;
}FRAMES_STATISTICS, *PFRAMES_STATISTICS;

typedef struct{
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int dayOfWeek;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
	unsigned int milliSecond;
}SYSTEMTIME;


//******************************//
//	error code list		//
//******************************//
#define ErrorCodeSuccess		0x0000


#define ErrorCodeDSPUninit		0x0001
#define ErrorCodeDSPNotReady		0x0002
#define ErrorCodeDSPLoadFail		0x0004
#define ErrorCodeDSPSyncObjectFail	0x0005
#define ErrorCodeDSPCountOverflow	0x0009
#define ErrorCodeEncodeChannelOverflow	0x000a
#define ErrorCodeDecodeChannelOverflow	0x000b
#define ErrorCodeBoardOverflow		0x000c
#define ErrorCodeDSPHexBlockLenOverflow	0x000d
#define ErrorCodeDSPProgramCheckoutFail	0x000e
#define ErrorCodeDSPInitRecheckFail	0x000f
#define ErrorCodeDSPBusy		0x0010

#define ErrorCodeNoCardInstalled	0x0102
#define ErrorCodeIoctlFail		0x0103
#define ErrorCodeMemLocateFail		0x0104
#define ErrorCodeDuplicateSN		0x0105
#define ErrorCodeCreateThreadFail	0x0106
#define ErrorCodeDSPCmdInvalid		0x0107
#define ErrorCodeHOSTCmdInvalid		0x0108
#define ErrorCodeDSPRespondCmdFail	0x0109
#define ErrorCodeOrderFail		0x010a
#define ErrorCodeKernelFail		0x010b
#define ErrorCodeStreamBufUnderflow	0x010c

#define ErrorCodeChannelOutofRange	0x0200
#define ErrorCodeInvalidEncodeChannel	0x0201
#define ErrorCodeInvalidArgument	0x0202
#define ErrorCodeNotSupport		0x0203
#define ErrorCodeMmapFail		0x0204

#define ErrorCodeCreateYUVOverlayFail	0x0301



  int InitDSPs();
  int DeInitDSPs();
  int ChannelOpen(int channelNum, STREAM_READ_CALLBACK streamReadCallback);
  int ChannelClose(int channelHandle);
  int GetTotalChannels();
  int GetTotalDSPs();
  
  int StartVideoPreview(int channelHandle, SDL_Surface *display, SDL_Rect dstRect);
  int StopVideoPreview(int channelHandle);

  int SetVideoPara(int channelHandel, int brightness,int contrast, int saturation, int hue);
  int GetVideoPara(int channelHandel, VideoStandard_t *videoStandard, int *brightness,int *contrast, int *saturation, int *hue);
  
  void GetSDKVersion(PVERSION_INFO versionInfo);
  int GetCapability(int channelHandle, CHANNEL_CAPABILITY *capability);
  int GetLastErrorNum();
  
  int SetStreamType(int channelHandle, int type);
  int GetStreamType(int channelHandle, int *streamType);

  int GetFramesStatistics(int channelHandle, PFRAMES_STATISTICS framesStatistics);
  int SetupMotionDetection(int channelHandle, RECT *RectList, int iAreas);
  int StartMotionDetection(int channelHandle);
  int GetBoardInfo(int channelHandle, unsigned int *boardType, char *serialNo);
  int StopMotionDetection(int channelHandle);
  
  int StartVideoCapture(int channelHandle);
  int StopVideoCapture(int channelHandle);
  int SetIBPMode(int hChannelHandle, int KeyFrameIntervals, int BFrames, int PFrames,int FrameRate);
  int SetDefaultQuant(int hChannelHandle, int IQuantVal, int PQuantVl, int BQuantVal);
 
  int SetOsd(int channelHandle, int enable);
  int SetAudioPreview(int channelHandle, int bEnable);
  int SetLogo(int channelHandle, int x, int y, int w, int h, unsigned char *yuv);
  int StopLogo(int channelHandle);

  int LoadYUVFromBmpFile(char *fileName, unsigned char *yuv, int bufLen, int *width, int *height);
  int SaveYUVToBmpFile(char *fileName, unsigned char *yuv, int width, int height);
 
  int GetOriginalImage(int channelHandle, unsigned char *imageBuf, int *size);
  int GetVideoSignal(int channelHandle);
  
  int MotionAnalyzer(int channelHandle, char *MotionRect, int iThreshold, int *iResult);
  int AdjustMotionDetectPrecision(int hChannelHandle, int iGrade, int iFastMotionDetectFps, int iSlowMotionDetectFps); 
  int CaptureIFrame(int channelHandle);
  int SetEncoderPictureFormat(int channelHandle, PictureFormat_t pictureFormat);
  int SetupBitrateControl(int channelHandle, unsigned long maxBps);
  int SetLogoDisplayMode(int channelHandle, unsigned short ColorKeyR, unsigned short ColorKeyG,
				unsigned short ColorKeyB, unsigned short bTranslucent, int TwinkleInterval);
  int SetOsdDisplayMode(int channelHandle, int brightness, int translucent, int twinkInterval,unsigned short *format1, unsigned short *format2);
  int SetVideoStandard(int channelHandle, VideoStandard_t videoStandard);
  
  int ResetDSP(int dspNumber);
  int GetSoundLevel(int channelHandle);

  int SetBitrateControlMode(int channelHandle, BitrateControlType_t brc);
  
  int SetupSubChannel(int channelHandle, int iSubChannel);
  int GetSubChannelStreamType(void *dataBuf, int frameType);


//new functions for DS40xxHC
  int SetupMask(int channelHandle, RECT *rectList, int iAreas);
  int StopMask(int channelHandle);
 
  int SetSubEncoderPictureFormat(int channelHandle, PictureFormat_t pictureFormat); 
  
  int StartSubVideoCapture(int channelHandle);
  int StopSubVideoCapture(int channelHandle);
 
  int SetupDateTime(int channelhandle, SYSTEMTIME *now);
  
  int SetInputVideoPosition(int channelHandle, unsigned int x, unsigned int y);

//add at 2005.1.13, compatible with DS400XH card
  int SetEncodePictureFormat(int channelHandle, PictureFormat_t pictureFormat);
//add at 2005.4.19 for getting the raw image stream
 typedef void* (*IMAGE_STREAM_CALLBACK)(unsigned int channelNumber, void* context);
  int RegisterImageStreamCallback(IMAGE_STREAM_CALLBACK pFunc, void* context);
  int SetImageStream(int channelHandle, int bStart, unsigned int fps, 
		     unsigned width, unsigned height,
	     	     unsigned char* imageBuffer);
#ifdef __cplusplus
        }
#endif
