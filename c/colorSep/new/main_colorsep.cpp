#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

using namespace cv;
using namespace std;

#include "colorsep.h"

Mat CamInputImg_raw, CamInputImg, Img_undistort, Img_Contour, Img_ColorSep, Img_corner, Img_UnRot;
Mat CamInputImg_rawhalf;
Mat cameraMatrix, distCoeffs;

int imgrows = 0, imgcols = 0;
vector<vector<Point> > contourSeq;

int FrameCounter = 0;
struct tm DisplayTime;
int ceramictype = 100;
char PostFoldName[256];
char ColorFoldName[256];
char SrcFoldName[256];
char CornerFoldName[256];
char LogFoldName[256];
char UnRotFoldName[256];
char RefFoldName[256];
char CurCamFileName[256];
char ClusterDataPath[256];

int IsCollapse = 0, IsDetectCollapse=1;

//进入白平衡调试模式
int IntoWBMode=0;
int WBDataType=0; //RGB0, LAB1,HSB2
Rect WBRect = Rect(0, 0, 1, 1);

int typevalue = 100;
int ProResize = 2;
int UseGLCM = 0;
int UseColorSep = 0;
int WithCalib = 1;
int UseNetConnect = 1;
int IsStart=0;
int InitSetPIC = 0;

int use_BWenhance = 0;
char CalibBWPath[100];
char CameraCalibPath[100];
char TrainFile[100];
char RunFile[100];
//CL表示Classic 传统的分类方式
int classifiertype = 0;         //0为采用传统的对比方式,1为分类器方式

vector<Descriptor> CL_RefSet;
vector<int> CL_typenum;
vector<int> CL_trainnum;

vector<int> KNN_Atypenum;
vector<int> KNN_Atrainnum;

vector<int> KNN_Ctypenum;
vector<int> KNN_Ctrainnum;

vector<float> totalMSG;

int MaxClass = 0;
int TrainSample=0;
char TrainTag[100] = "";
int TrainMode = 0, TrainType = 0;//, IsFileTrain = 1;
int useClusterSep=0, useClusterData=0;
int ClusterNum=0, IsClusterMerge=0, Cluster_svFlag=0;

struct timeval Alarm_Time;
struct timeval Total_Time;
int Process_Time = 1;


void Init_DataMem()
{
  CL_typenum.clear();
  CL_trainnum.clear();
  CL_RefSet.clear();

  KNN_Atrainnum.clear();
  KNN_Ctrainnum.clear();
  KNN_Ctypenum.clear();
  KNN_Atypenum.clear();

  CamInputImg_raw.create(imgrows, imgcols, CV_8UC3);
  CamInputImg.create(imgrows/ProResize, imgcols/ProResize, CV_8UC3);
  Img_undistort.create(imgrows/ProResize, imgcols/ProResize, CV_8UC3);
  Img_Contour.create(500, 600, CV_8UC3);
  //所有分色用的图片尺寸约束成一致的
  Img_ColorSep.create(800, 800, CV_8UC3);
  
}

int Ref_fdstat = -1, Img_fdstat = -1;
int Ref_Tcpfd=-1, Img_Tcpfd=-1;


void Init_TCP()
{
  Ref_fdstat = InitRefTcp(Ref_Tcpfd);
  Img_fdstat = InitImageTcp(Img_Tcpfd);

  cout << "The NetStat is: Ref_tcp_ " << Ref_fdstat<< " with socket " << Ref_Tcpfd
       <<", and the image_tcp is _" << Img_fdstat << " with socket " << Img_Tcpfd << endl;
 
}

  char window_name0[] = "img_color";
  char window_name1[] = "img_H";
	  char window_name2[] = "img_S";
	  char window_name3[] = "img_V";

void SystemInit(int type)
{
  namedWindow("jai_opencv",0);

   // namedWindow (window_name0,
   // 	  	       WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
   // 	  namedWindow (window_name1,
   // 	  	       WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
   // 	  namedWindow (window_name2,
   // 	  	       WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
   // 	  namedWindow (window_name3,
   // 	  	       WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
	  
  
  if(type == 1)
    Init_ImgPara();
 
  
  Init_DataMem();
  if(WithCalib)
    Init_Camera();
  
  GetCurTime(&DisplayTime);
  char foldname[256];
  strcpy(foldname, "PostFile");
  
  Init_Fold(foldname, PostFoldName);
  
  strcpy(foldname, "ColorFile");
  Init_Fold(foldname, ColorFoldName);
  
  
  strcpy(foldname, "SrcFile");
  Init_Fold(foldname, SrcFoldName);

  strcpy(foldname, "CornerFile");
  Init_Fold(foldname, CornerFoldName);

  strcpy(foldname, "UnRotateFile");
  Init_Fold(foldname, UnRotFoldName);

  
  strcpy(foldname, "LogFile");
  Init_Fold(foldname, LogFoldName);
  
  strcpy(foldname, "RefFile");
  Init_Fold(foldname, RefFoldName);
  
  
  Init_log(LogFoldName);

  
}



void SavingFile(Mat& Img, int Counter);


void RefManage(Mat& New_Image, int totalNum)
{
  if(UseNetConnect)
    {
      if(Ref_fdstat ==-1)
	{
	  Ref_fdstat = InitRefTcp(Ref_Tcpfd);
	  cout << "No Ref connection, reconnect" << endl;
	}
      else
	sendNewRefImage(totalNum, Ref_Tcpfd);
	//sendNewRefImage(totalNum, Ref_Tcpfd);
    }

  SavingFile(New_Image, totalNum, RefFoldName);
  cout << "SavingFile Ref" << endl;

  return;
}


Mat BW_weight, BW_weight_half;

int main(int argc,char** argv)
{
  cout << "Usage :: \n ./Colorsep 或者 ./Colorsep filename " << endl;
  char parafile[] = "para/system.conf";
  ReadSysConf(parafile);
  

  if(UseNetConnect)
    {
      Init_TCP();
      pthread_t RecvCmd;
      pthread_create(&RecvCmd,NULL, RecvInterface,(void*)NULL);
    }
  
  
  while(!IsStart && UseNetConnect)
    {
      sleep(1);
    }
  
 
  cout << "1" << endl;

  //开启图像获取进程
  
  if( InitSetPIC == 1)
    {
      cout << "Init SetPic " << endl;
      char co_proc[100]="/workSpace/2012-12-03/SetPic/SetPic ";
      strcat(co_proc, TrainFile); 
      strcat(co_proc, " "); 
      strcat(co_proc, RunFile);

      cout << "co_proc: " << co_proc << endl;
      FILE *readPIC = popen(co_proc, "r");

      usleep(200000);
    }

  if(argc == 1)
    {
      if( 0 != Init_SHM())
	{
	  cout << "SHM initial Failed, Program exit!" << endl;
	  exit(0);
	}
    }
  else
    {
      CamInputImg_raw = imread(argv[1], 1);
      imgcols = CamInputImg_raw.cols;
      imgrows = CamInputImg_raw.rows;

    }
  
  //根据从配置文件，以及共享内存读取到的初始参数，初始化内存
  SystemInit(argc);
  
  if(use_BWenhance && WithCalib)
    {
      Mat calibImg = imread(CalibBWPath, 1);
      gen_BWweight(calibImg, BW_weight);
      pyrDown(BW_weight, BW_weight_half);
    }
  
  int findInitSeq = 0;

  while(true)
    {
      if(argc == 1 )
	PicData(CamInputImg_raw, CurCamFileName);
      
      //界面控制，进入白平衡设置模式
      cout << "Done PicData with IntoWBMode " << IntoWBMode <<  endl;
      if(IntoWBMode)
	{
	  DataForDisp CurDisp;
	  Mat DispImage;
	  CurDisp.sequence = (int)FrameCounter;
	  	  
	  AreaAvgDisp(CamInputImg_raw, Img_Contour.size(), DispImage);
	  CurDisp.DispImage=DispImage.clone();
	  //ut << "CurDisp.DispImage " << CurDisp.DispImage.cols << ", " << CurDisp.DispImage.rows << endl;
	  
	  //CurDisp.DispImage = Img_Contour.clone();

	  //cout << "DebugData is " << Mat(totalMSG) << endl;
	  
	  if(UseNetConnect)
	    {
	      if (Img_fdstat==-1)
		{
		  Img_fdstat=InitImageTcp(Img_Tcpfd); 
		}
	      else
		int isOK = sendImage(&CurDisp, Img_Tcpfd);
	    }
	  continue;
	}


      if(Process_Time )
	{
	  gettimeofday(&Alarm_Time, NULL);
	  gettimeofday(&Total_Time, NULL);
	}
      
      if(TrainMode && findInitSeq == 0)
	{
	  cout << "Current Filename is " << CurCamFileName << endl;

	  if(TrainTag[0] == '\0')
	    {
	      findInitSeq = 1;
	      cout << "No TrainTag, the first Img is used as first train sample" << endl;
	    }
	  else
	    {
	      if(strstr(CurCamFileName, TrainTag) == NULL)
		{
		  cout << "continue to the next file with CurCamFileName :" <<  CurCamFileName << " while TrainTag " << TrainTag << endl;
		  continue;
		}
	      else
		{
		  cout <<"Program Started: the init file is found" << endl;
		  
		  findInitSeq = 1;
		}
	    }
	}
      
      cout << "\n\n================================Current Frame " << ++FrameCounter 
	   << " with File Name " << CurCamFileName << "=========================\n" << endl;

      
      if(WithCalib)
	{
	  if(Process_Time )
	    gettimeofday(&Alarm_Time, NULL);
	  
	  if(use_BWenhance)
	    {
	      //enhance_calib(BW_weight, CamInputImg_raw, CamInputImg);
	      pyrDown(CamInputImg_raw, CamInputImg_rawhalf);
	      enhance_calib(BW_weight_half, CamInputImg_rawhalf, CamInputImg);
	    }
	  else
	    pyrDown(CamInputImg_raw, CamInputImg);
	  
	  
	  if(Process_Time)
	    DiffTime(&Alarm_Time, "PicData && BWenhance && pyrDown", FrameCounter);
	  
	  if(Process_Time )
	    gettimeofday(&Alarm_Time, NULL);
	  
	  Undistort(CamInputImg, Img_undistort, cameraMatrix, distCoeffs);
	  
	  if(Process_Time)
	    DiffTime(&Alarm_Time, "Undistort", FrameCounter);
	  
	  if(Process_Time )
	    gettimeofday(&Alarm_Time, NULL);
      
	  CeramicSeg(Img_undistort, Img_Contour, Img_ColorSep, Img_corner, Img_UnRot, contourSeq, UseColorSep);
	  
	  if(Process_Time)
	    DiffTime(&Alarm_Time, "CeramicSeg", FrameCounter);
	}
      else
	{
	  cout << " Process File WithOut Calibration\n" << endl;
	  IsCollapse = 0;
	  resize(CamInputImg_raw, Img_Contour, Img_Contour.size());
	  resize(CamInputImg_raw, Img_ColorSep, Img_ColorSep.size());
	}
      
      imshow("jai_opencv", Img_Contour);
      waitKey(20);
      
      
      if(Process_Time )
	gettimeofday(&Alarm_Time, NULL);
      
      //cout << "CeramicSeg Done\n" << endl;
      //训练已经完成
      if(TrainSample*TrainType < FrameCounter && TrainMode != 0)
	TrainMode = 0;
      
      if(FrameCounter == 1 && useClusterSep)
	{
	  int getData = 1;
	  if(useClusterData)
	    getData = LoadClusterData(ClusterDataPath, IsClusterMerge, Cluster_svFlag);

	  if(!getData || !useClusterData )
	    clusterTrain(Img_ColorSep, ClusterDataPath, IsClusterMerge, Cluster_svFlag);

	  clusterTrain_finished(ClusterNum, IsClusterMerge);
	}
      
      if(Process_Time)
	DiffTime(&Alarm_Time, "clusterTrain", FrameCounter);
      
      if(Process_Time )
	gettimeofday(&Alarm_Time, NULL);

      
      vector<Descriptor> Img_Des;
      Descriptor Des0;

      if(useClusterSep)
	{
	  vector<Mat> new_mask;
	  vector<Mat> category;
	  
	  Mat finalCenter = calcuCenter(Img_ColorSep, ClusterNum, IsClusterMerge, Cluster_svFlag, new_mask, category);
	  
	  cout << "finalCenter " << finalCenter << endl;

	  for(int i=0; i<new_mask.size(); i++)
	    {
	      calcImagevalue(category[i], new_mask[i], Des0);
	      Img_Des.push_back(Des0);
	    }
	  cout << 1 <<endl;
	}
      else
	{
	  Mat myMask=255*Mat::ones(Img_ColorSep.size(), CV_8UC1);
	  
	  calcImagevalue(Img_ColorSep, myMask, Des0);
	  Img_Des.push_back(Des0);
	}
	          	  
      if(Process_Time)
	DiffTime(&Alarm_Time, "calc Descriptor", FrameCounter);
      
      if(TrainMode != 0)
	{
	  printf("Under Train Mode: MaxClass: %d, with traintype: %d\n", MaxClass, TrainMode);
	  
	  if(classifiertype == 0)
	    {
	      int RefNum = 0;
	      int IsRefMan = CL_Train(Img_Des[0], FrameCounter,  TrainSample, RefNum);
	      if(IsRefMan)
		RefManage(Img_ColorSep, RefNum);
	    }
	  else if(classifiertype == 1)
	    {
	      vector<float> KnnData, tempData;
	      if(useClusterSep)
		{
		  for(int i=0; i<Img_Des.size(); i++)
		    {
		      GenKNN_trainData(Img_Des[i], tempData, Cluster_svFlag);

		      for(int j=0; j<tempData.size(); j++)
			KnnData.push_back(tempData[j]);
		    }
		  cout << "my KnnData is " << Mat(KnnData) << endl;
		}
	      else
		GenKNN_trainData(Img_Des[0], KnnData, Cluster_svFlag);
	      
	      if(FrameCounter == 1)
		KNN_datainit(TrainType*TrainSample, KnnData.size(), TrainType);
	      
	      typevalue = KNN_Train(KnnData, FrameCounter, TrainSample);
	      
	      cout << "KNN_Train " << typevalue << endl;
	      
	    }
	  //cout << "Process_sep Done\n" << endl;
	  else
	    cout << "ERROR, no type of Classifiertype" << endl;

	  //Last Train File
	  if(FrameCounter == TrainType*TrainSample)
	    {
	      if(classifiertype == 1)
		{
		  char reflabel[256]="";
		  KNN_trainfinished(MaxClass, TrainType, reflabel);
		  sendInitRefImage(TrainType, reflabel, Ref_Tcpfd);
		}
	    }
	}
      else
	{
	  printf("Under Process Mode: MaxClass: %d\n", MaxClass);
	  if(classifiertype == 0)
	    {
	      int IsRefMan = 0;
	      typevalue = CL_ProcessSep(Img_Des[0], FrameCounter, MaxClass, IsRefMan);

	      if(IsRefMan)
		RefManage(Img_ColorSep, typevalue+1);
	    }
	  else if(classifiertype == 1)
	    {

	      vector<float> KnnData, tempData;
	      if(useClusterSep)
		{
		  for(int i=0; i<Img_Des.size(); i++)
		    {
		      GenKNN_trainData(Img_Des[i], tempData, Cluster_svFlag);

		      for(int j=0; j<tempData.size(); j++)
			KnnData.push_back(tempData[j]);
		    }
		}
	      else
		GenKNN_trainData(Img_Des[0], KnnData, Cluster_svFlag);
	      
	      normalizeVec(KnnData);
	      typevalue = KNN_ProcessSep(KnnData, FrameCounter, MaxClass);
	    }
	       //cout << "Process_sep Done\n" << endl;
	  else
	    cout << "ERROR, no type of Classifiertype" << endl;

	}
     
      
 
      if(Process_Time)
	DiffTime(&Alarm_Time, "Process_sep", FrameCounter);


      if(Process_Time )
	gettimeofday(&Alarm_Time, NULL);


      SavingFile(Img_Contour, FrameCounter, PostFoldName);
	    
      SavingFile(Img_ColorSep, FrameCounter, ColorFoldName);
      //SavingFile(CamInputImg_raw, FrameCounter, SrcFoldName);

      cout << 2<< endl;
      if(WithCalib && (FrameCounter % 20 == 0 || FrameCounter < 100))
	{
	  Mat SaveTemp;
	  pyrDown(Img_UnRot, SaveTemp);
	  SavingFile(SaveTemp, FrameCounter, UnRotFoldName);
	  
	  pyrDown(Img_corner, SaveTemp);
	  SavingFile(SaveTemp, FrameCounter, CornerFoldName);
	}
      SavingLog(FrameCounter, CurCamFileName);

      if(Process_Time)
	DiffTime(&Alarm_Time, "Saving File", FrameCounter);
      
      //ResultMSG MSG;
      //SentToNet(MSG);
      
      //UpDateRef(int );

      DataForDisp CurDisp;
      CurDisp.sequence = (int)FrameCounter;
      CurDisp.types = typevalue;
      if(classifiertype == 0)
	CurDisp.typessequence = CL_typenum[typevalue];
      else if(classifiertype == 1)
	CurDisp.typessequence = KNN_Atypenum[typevalue];
      
      CurDisp.collapse = IsCollapse;
      CurDisp.debugData = totalMSG;
      
      Mat Img_R(Img_Contour.size(), CV_8UC1);
      Mat Img_G(Img_Contour.size(), CV_8UC1);
      Mat Img_B(Img_Contour.size(), CV_8UC1);
      Mat Img_BGR(Img_Contour.size(), CV_8UC3);

      vector<Mat> TestRGB(3), TestBGR(3);
      TestRGB[0]= Img_R, TestRGB[1]= Img_G, TestRGB[2]= Img_B;
      TestBGR[0]= Img_B, TestBGR[1]= Img_G, TestBGR[2]= Img_R;
  
      split(Img_Contour, TestRGB);
      merge(TestBGR, Img_BGR);

      //CurDisp.DispImage = Img_Contour.clone();
      CurDisp.DispImage = Img_BGR.clone();

      //cout << "DebugData is " << Mat(totalMSG) << endl;

      if(UseNetConnect)
	{
	  if (Img_fdstat==-1)
	    {
	      Img_fdstat=InitImageTcp(Img_Tcpfd); 
	    }
	  else
	    int isOK = sendImage(&CurDisp, Img_Tcpfd);
	}

      if(Process_Time)
	DiffTime(&Total_Time, "Program Totol Time", FrameCounter);
      
    }
  
  cvDestroyWindow("jai_opencv");

  return 0;
}
  
