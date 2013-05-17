/*
 * =====================================================================================
 *
 *       Filename:  options.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 09, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "options.h"
#include "ini.h"
#include "debug.h"

static int parse_params_action(const char *argv[], int ii, Tag t)
{
    int skip;
    skip = 0;

    switch (t.type) {
        case TAGTYPE_BOOL:
            *((int*)t.data) = 1;
            break;
        case TAGTYPE_INT:
            if (argv[ii+1]==NULL) return PARSE_ERROR;
            *((int*)t.data) = atoi(argv[ii+1]);
            skip = 1;
            break;
        case TAGTYPE_FLOAT:
            if (argv[ii+1]==NULL) return PARSE_ERROR;
            *((float*)t.data) = atof(argv[ii+1]);
            skip = 1;
            break;
        case TAGTYPE_STRING:
            if (argv[ii+1]==NULL) return PARSE_ERROR;
            strcpy((char*)t.data, argv[ii+1]);
            skip = 1;
            break;
    }

    return skip;
}

static int parse_params(int argc, const char *argv[], int numtags, Tag* t)
{
    int ii, ij, status, count;

    for (ii=0; ii<numtags; ii++) {
        if (strcmp(t[ii].name,"")==0)
            return TAGERR_NAME;
        if (t[ii].type > TAGTYPE_STRING)
            return TAGERR_TYPE;
        if (t[ii].data == NULL)
            return TAGERR_DATA;
    }

    count = 0;
    for (ii=1; ii<argc; ii++) {
        for (ij=0; ij<numtags; ij++) {
            if (strcmp(t[ij].name, argv[ii])==0) {
                status = parse_params_action(argv, ii, t[ij]);
                if (status<0) {
                    return status;
                }

                ii += status;
                ij = numtags;
                count++;
            }
        }
    }
    return count;
}

static int ini_handler(void* user, const char* section, 
        const char* name, const char* value)
{
    Ini* ini = (Ini*) user;

    if (MATCH(name, "CameraFile")) {
        strcpy(ini->CameraFile, value);
    } else if (MATCH(name, "UseBWenh")) {
        ini->UseBWenh = atoi(value);
    } else if (MATCH(name, "CalibBWPath")) {
        strcpy(ini->CalibBWPath, value);
    } else if (MATCH(name, "RunFile")) {
        strcpy(ini->RunFile, value);
    } else if (MATCH(name, "UseClusterSep")) {
        ini->UseClusterSep = atoi(value);
    } else if (MATCH(name, "UseClusterData")) {
        ini->UseClusterData = atoi(value);
    } else if (MATCH(name, "ClusterDataPath")) {
        strcpy(ini->ClusterDataPath, value);
    } else if (MATCH(name, "ParaNum1")) {
        ini->ParaNum1 = atof(value);
    } else if (MATCH(name, "ParaNum2")) {
        ini->ParaNum2 = 3.14/180*atof(value);
    } else if (MATCH(name, "DetectCollapse")) {
        ini->DetectCollapse = atoi(value);
    } else if (MATCH(name, "C_thresh")) {
        ini->C_thresh = atof(value);
    } else if (MATCH(name, "QUIET_LEVEL")) {
        ini->QUIET_LEVEL = atoi(value);
    } else if (MATCH(name, "PROCESS_TIME")) {
        ini->PROCESS_TIME = atoi(value);
    } else if (MATCH(name, "Resize_Mode")) {
        ini->Resize_Mode = atoi(value);
    } else if (MATCH(name, "UseNetConnect")) {
        ini->UseNetConnect = atoi(value);
    } else if (MATCH(name, "UseGLCM")) {
        ini->UseGLCM = atoi(value);
    } else if (MATCH(name, "WithCalib")) {
        ini->WithCalib = atoi(value);
    } else if (MATCH(name, "InitSetPIC")) {
        ini->InitSetPIC = atoi(value);
    } else if (MATCH(name, "UseColorSep")) {
        ini->UseColorSep = atoi(value);
    } else if (MATCH(name, "UseSubThresh")) {
        ini->UseSubThresh = atoi(value);
    } else if (MATCH(name, "Subthresh_Seppart")) {
        ini->Subthresh_Seppart = atoi(value);
    } else if (MATCH(name, "Classifier")) {
        ini->Classifier = atoi(value);
    } else if (MATCH(name, "TrainFile")) {
        strcpy(ini->TrainFile, value);
    } else if (MATCH(name, "MaxClass")) {
        ini->MaxClass = atoi(value);
    } else if (MATCH(name, "TrainType")) {
        ini->TrainType = atoi(value);
    } else if (MATCH(name, "TrainTag")) {
        strcpy(ini->TrainTag, value);
    } else if (MATCH(name, "TrainSample")) {
        ini->TrainSample = atoi(value);
    } else {
        return 0;
    }

    return 1;
}

static void initOptions(Options *options)
{
    memset(options, 0, sizeof(Options));
    CmdLine *cmd = &(options->cmd);
    Ini *ini = &(options->ini);

    //////////////////  命令行参数默认配置 //////////////////
    /* 配置文件路径 */
    strcpy(cmd->config_file_path, "");

    /* tcp服务器地址 */
    strcpy(cmd->server_ip_addr, DEFAULT_SERVER_IP);

    /* tcp服务器端口 */
    cmd->server_port = DEFAULT_SERVER_PORT;

    /* 打印帮助信息并退出 */
    cmd->help_mode = 0;

    //////////////////  配置文件默认配置 //////////////////
    strcpy(ini->CameraFile, "~/SrcImg/1225//CameraCalib.xml");
    ini->UseBWenh = 1;
    strcpy(ini->CalibBWPath, "~/SrcImg/1225/BW_calib.bmp");
    strcpy(ini->RunFile, "/workSpace/2012-12-03/SetPic/run_campinya.conf");
    ini->UseClusterSep = 1;
    ini->UseClusterData = 1;
    strcpy(ini->ClusterDataPath, "/workSpace/2012-12-03/GetPic/para/campinya_cluster.xml");
    ini->ParaNum1 = 3;
    ini->ParaNum2 = 3.14/180*0.8;
    ini->DetectCollapse = 1;
    ini->C_thresh = 0.9;
    ini->QUIET_LEVEL = 2;
    ini->PROCESS_TIME = 1;
    ini->Resize_Mode = 0;
    ini->UseNetConnect = 0;
    ini->UseGLCM = 0;
    ini->WithCalib = 0;
    ini->InitSetPIC = 1;
    ini->UseColorSep = 1;
    ini->UseSubThresh = 1;
    ini->Subthresh_Seppart = 7;
    ini->Classifier = 1;
    strcpy(ini->TrainFile, "/workSpace/2012-12-03/SetPic/train_campinya.conf");
    ini->MaxClass = 8;
    ini->TrainType = 3;
    strcpy(ini->TrainTag, "");
    ini->TrainSample = 10;
}

static void dumpOptions(Options *options)
{
    DEBUG("-----------dump command line ------------\n");
    DEBUG("config_file_path: %s\n", options->cmd.config_file_path);
    DEBUG("server_ip_addr: %s\n", options->cmd.server_ip_addr);
    DEBUG("server_port: %d\n", options->cmd.server_port);
    DEBUG("-----------dump ini config --------------\n");
    DEBUG("CameraFile: %s\n", options->ini.CameraFile);
    DEBUG("UseBWenh: %d\n", options->ini.UseBWenh);
    DEBUG("CalibBWPath: %s\n", options->ini.CalibBWPath);
    DEBUG("RunFile: %s\n", options->ini.RunFile);
    DEBUG("UseClusterSep: %d\n", options->ini.UseClusterSep);
    DEBUG("UseClusterData: %d\n", options->ini.UseClusterData);
    DEBUG("ClusterDataPath: %s\n", options->ini.ClusterDataPath);
    DEBUG("ParaNum1: %f\n", options->ini.ParaNum1);
    DEBUG("ParaNum2: %f\n", options->ini.ParaNum2);
    DEBUG("DetectCollapse: %d\n", options->ini.DetectCollapse);
    DEBUG("C_thresh: %f\n", options->ini.C_thresh);
    DEBUG("QUIET_LEVEL: %d\n", options->ini.QUIET_LEVEL);
    DEBUG("PROCESS_TIME: %d\n", options->ini.PROCESS_TIME);
    DEBUG("Resize_Mode: %d\n", options->ini.Resize_Mode);
    DEBUG("UseNetConnect: %d\n", options->ini.UseNetConnect);
    DEBUG("UseGLCM: %d\n", options->ini.UseGLCM);
    DEBUG("WithCalib: %d\n", options->ini.WithCalib);
    DEBUG("InitSetPIC: %d\n", options->ini.InitSetPIC);
    DEBUG("UseColorSep: %d\n", options->ini.UseColorSep);
    DEBUG("UseSubThresh: %d\n", options->ini.UseSubThresh);
    DEBUG("Subthresh_Seppart: %d\n", options->ini.Subthresh_Seppart);
    DEBUG("Classifier: %d\n", options->ini.Classifier);
    DEBUG("TrainFile: %s\n", options->ini.TrainFile);
    DEBUG("MaxClass: %d\n", options->ini.MaxClass);
    DEBUG("TrainType: %d\n", options->ini.TrainType);
    DEBUG("TrainTag: %s\n", options->ini.TrainTag);
    DEBUG("TrainSample: %d\n", options->ini.TrainSample);
}

static void print_usage()
{
    printf("ColorSepServer [OPTIONS]    \n");
    printf("                            \n");
    printf("Options:                    \n");
    printf("--config            file path of sysconf\n");
    printf("--addr              server IP address\n");
    printf("--port              server port\n");
    printf("-h,--help           print this message\n");

    exit(2);
}

int getOptions(Options *options, int argc, const char *argv[])
{
    int rtn;
    static Tag tag[] = {
        {"--config", TAGTYPE_STRING, options->cmd.config_file_path},
        {"--addr", TAGTYPE_STRING, options->cmd.server_ip_addr},
        {"--port", TAGTYPE_INT, &options->cmd.server_port},
        {"-h", TAGTYPE_BOOL, &options->cmd.help_mode},
        {"--help", TAGTYPE_BOOL, &options->cmd.help_mode},
    }; 

    initOptions(options);

    int size = (sizeof(tag)/sizeof(Tag));
    rtn = parse_params(argc, argv, size, tag);

    if (options->cmd.help_mode) {
        print_usage();
    }

    if (strcmp(options->cmd.config_file_path, "") != 0) {
        if (ini_parse(options->cmd.config_file_path, ini_handler, &(options->ini)) < 0) {
            fprintf(stderr, "getOptions: load ini error\n");
            return -1;
        }
    } else {
        printf("getOptions: use default sysconf\n");
    }

    dumpOptions(options);

    DEBUG("get command line arg num: %d\n", rtn);

    return rtn;
}
