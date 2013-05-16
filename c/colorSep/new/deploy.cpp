#include "ctype.h"

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
#include <stdlib.h>
#include <sys/time.h> //用于系统计时


//配置文件的大小要求
#define MAXLINES 200
#define MAXLINELENGTH 150

using namespace std;

typedef struct _pfile
{
    // This structure is for every line in the config file
    struct	
    {
	// if the following three variable are all NULL a blank line is assumed
	char key[30]; 		// key  e.g KEY=
	char value[80];     // value e.g KEY="VALUE"
    } data[MAXLINES];

    int length;				// total number of line in config file
} DataStore;

extern int Debug_Mode;
extern int resize_mode;
extern int UseNetConnect;
extern int UseGLCM;
extern int InitSetPIC;
extern int use_subthresh;
extern int UseColorSep;
extern int Subthresh_Seppart;
extern int use_BWenhance;
extern char CalibBWPath[100];
extern int useClusterData;
extern char ClusterDataPath[256];
extern int useClusterSep;

extern char CameraCalibPath[100];
extern char TrainFile[100];
extern char RunFile[100];
extern int classifiertype;         //0为采用传统的对比方式,1为分类器方
extern int MaxClass;
extern int TrainSample;
extern int TrainMode, TrainType;//, IsFileTrain;
extern char TrainTag[100];

extern float colorDis;
extern float sepangle;
extern float collapse_thresh;
extern int IsDetectCollapse;
extern int Process_Time;
extern int WithCalib;

extern int classNum, classNumSys;

int parse(char *filename, DataStore* pfile)
{
    FILE *fptr;
    int i, j, lineno, qf;
    char linebuf[MAXLINELENGTH];
    char keyvaluebuf[132];
    char *char_ptr;
    
    fptr = fopen(filename, "rt");
    
    if(fptr == 0)
	return -1;


    for(lineno = 0; lineno < MAXLINES; lineno++)
    {
	if( fgets(linebuf, MAXLINELENGTH, fptr) == NULL )
	    break;
	

	if((char_ptr = strchr(linebuf, '#')) != NULL)
	{
	    *char_ptr = 0;
	    continue;
	}
	
	for(i = j = qf = 0; linebuf[i] != 0; i++)
	{
	   
	    if(linebuf[i] == '\"')
	    {
		qf = !qf;
	    }
	    else
	    {
		if(!isspace(linebuf[i]) || qf)
		{
		    keyvaluebuf[j++] = linebuf[i];
		}
	    }
	}
	
	keyvaluebuf[j++] = 0;
	

	if((char_ptr = strchr(keyvaluebuf, '=')) != NULL)
	{
	    strcpy(pfile->data[lineno].value, char_ptr + 1);
	    *char_ptr = 0;
	    strcpy(pfile->data[lineno].key, keyvaluebuf);
	}
    }
    pfile->length = lineno;
    fclose(fptr);
    
    return lineno;
}

//returns pointer to value for supplied key. If key is not found returns zero.
char keyvalue[80];

char *getkeyvalue(const char *key, DataStore pfile, int forcestat, int &valuestat)
{
    int lineno;
    // search through all line of pfile for key value
    for(lineno = 0; lineno < pfile.length; lineno++)
    {
	if(strcmp(key, pfile.data[lineno].key) == 0) // if key is found, exit loop.
	    break;
    }

    if(lineno == pfile.length ) // not found
    {
      printf("匹配字符%s没有找到！\n", key);
	
	valuestat = 0;
	if(forcestat)
	    exit(1);
    }
    else
    {
	valuestat = 1;
	strcpy(keyvalue, pfile.data[lineno].value);
    }

    return keyvalue;
}

void ReadSysConf(char *ParFileName)
{
    char key[32];
    char getvalue[80];
    int forcevalue = 1;
    int valuestat;

    DataStore pfile;
    // 清空Buffer
    memset(&pfile, 0, sizeof(pfile));
    
    if(parse(ParFileName, &pfile) < 0)
    {
	perror(ParFileName);
	exit(1);
    }
    
    
    Debug_Mode = atoi(getkeyvalue("QUIET_LEVEL", pfile, forcevalue, valuestat));
    resize_mode = atoi(getkeyvalue("Resize_Mode", pfile, forcevalue, valuestat));
    UseNetConnect = atoi(getkeyvalue("UseNetConnect", pfile, forcevalue, valuestat));
    UseGLCM = atoi(getkeyvalue("UseGLCM", pfile, forcevalue, valuestat));
    
    colorDis = atof(getkeyvalue("ParaNum1", pfile, forcevalue, valuestat));
    sepangle = 3.14/180*atof(getkeyvalue("ParaNum2", pfile, forcevalue, valuestat));
    collapse_thresh = atof(getkeyvalue("C_thresh", pfile, forcevalue, valuestat));
    IsDetectCollapse = atoi(getkeyvalue("DetectCollapse", pfile, forcevalue, valuestat));
    // cout << colorDis << "and " << sepangle << endl;
    printf("colorDis is %f, and sepangle is %f\n", collapse_thresh, sepangle);

    use_BWenhance = atoi(getkeyvalue("UseBWenh", pfile, forcevalue, valuestat));
    strcpy(CalibBWPath, getkeyvalue("CalibBWPath", pfile, forcevalue, valuestat));

    strcpy(CameraCalibPath, getkeyvalue("CameraFile", pfile, forcevalue, valuestat));
    strcpy(TrainFile, getkeyvalue("TrainFile", pfile, forcevalue, valuestat));
    strcpy(RunFile, getkeyvalue("RunFile", pfile, forcevalue, valuestat));
    
    WithCalib = atoi(getkeyvalue("WithCalib", pfile, forcevalue, valuestat));
    useClusterData=atoi(getkeyvalue("UseClusterData", pfile, forcevalue, valuestat));

    useClusterSep=atoi(getkeyvalue("UseClusterSep", pfile, forcevalue, valuestat));

    if(useClusterData)
      strcpy(ClusterDataPath, getkeyvalue("ClusterDataPath", pfile, forcevalue, valuestat));
    
    use_subthresh = atoi(getkeyvalue("UseSubThresh", pfile, forcevalue, valuestat));
    UseColorSep = atoi(getkeyvalue("UseColorSep", pfile, forcevalue, valuestat));
    Subthresh_Seppart = atoi(getkeyvalue("Subthresh_Seppart", pfile, forcevalue, valuestat));
    InitSetPIC = atoi(getkeyvalue("InitSetPIC", pfile, forcevalue, valuestat));
    
    MaxClass = atoi(getkeyvalue("MaxClass", pfile, forcevalue, valuestat));

    TrainType = atoi(getkeyvalue("TrainType", pfile, forcevalue, valuestat));
    classNum = TrainType-1;
    classNumSys = TrainType-1;


    TrainSample = atoi(getkeyvalue("TrainSample", pfile, forcevalue, valuestat));
    strcpy(TrainTag, getkeyvalue("TrainTag", pfile, forcevalue, valuestat));
    //IsFileTrain = atoi(getkeyvalue("IsFileTrain", pfile, forcevalue, valuestat));

    classifiertype = atoi(getkeyvalue("Classifier", pfile, forcevalue, valuestat));
    Process_Time = atoi(getkeyvalue("PROCESS_TIME", pfile, forcevalue, valuestat));

    if(TrainType == 0 && classifiertype != 0 && UseNetConnect == 0)
      {
	printf("Error TrainType with classifiertype when use local mode\n");
	exit(0);
      }
    TrainMode = TrainType;
    
    if(classifiertype != 0 && classifiertype != 1)
      {
	printf("Classifier ERROR\n");
	exit(0);
      }
    return;
}
