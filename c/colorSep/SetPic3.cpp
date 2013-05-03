//g++ -o SetPic SetPic.cpp -lboost_date_time -lboost_filesystem -lboost_system `pkg-config opencv --libs --cflags opencv`

#include <iostream>
#include <string>
#include <sys/shm.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;


#define BOOST_SYSTEM_NO_LIB
#define BOOST_FILESYSTEM_NO_LIB
#include <boost/filesystem.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/filesystem/operations.hpp>
#define BOOST_DATE_TIME_SOURCE
#define BOOST_DATE_TIME_NO_LIB
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/typeof/typeof.hpp>
//using namespace boost;
using namespace boost::gregorian;
using namespace boost::posix_time;

using namespace boost;
using namespace boost::filesystem;
using namespace boost::xpressive;
using namespace boost::system;

#include "TypeDef.h"

#include <sstream>
#include <fstream>
//#include <string>


typedef recursive_directory_iterator rd_iterator;
static void find_files(const path& dir,const string& filename,vector<path>& v)   //查找
{
    static boost::xpressive::sregex_compiler rc;
    if(!rc[filename].regex_id())
    {
        string str = boost::replace_all_copy(boost::replace_all_copy(filename,".","\\."),"*",".*");
        rc[filename] = rc.compile(str);
    }
    typedef vector<path> result_type;
    if(!exists(dir) || !is_directory(dir)) return ;

    rd_iterator end;
    for(rd_iterator pos(dir); pos != end; ++pos)
    {
        if(!is_directory(*pos) && regex_match(pos->path().string(),rc[filename]))
            v.push_back(pos->path());
    }
}


bool SortFunc(path p1,path p2)
{
    string str1(p1.filename().c_str());
    string str2(p2.filename().c_str());
    bool retval(strcmp(str1.c_str(),str2.c_str())<0);
    return retval;
}



int main(int argc,char** argv)
{
    std::vector<std::string> vInput;
    int nCount(argc>=7?argc:7);
    vInput.reserve(nCount);
    //vInput.resize(nCount,"");
    int _index(0);
    while(_index<argc)
    {
        vInput.push_back(argv[_index]);
        ++_index;
    }
    int x(0),y(0),offset_x(0),offset_y(0);
    //path dir = "./../IMG/";
    std::string strInputFilename=(argc>=2)?std::string(vInput[1]):"train.conf";
    std::string strInputRunname=(argc>=3)?std::string(vInput[2]):"./../IMG";    //默认设置 为 ./../IMG
    boost::filesystem::path dir(strInputRunname);    
    bool bReadInputFile(true);
    bool bIsDir(true);

    if(strInputFilename.empty())
    {
        bReadInputFile=false;
    }
    else
    {
        boost::filesystem::path pa(strInputFilename.c_str());
        if(!boost::filesystem::exists(pa)) 
        {
            fprintf(stderr, "[**** ERROR] File Not Exists :%s \n",strInputFilename.c_str());
            bReadInputFile=false;
        }
        else
        {
            fprintf(stdout,">>>>>  TRAIN:%s\n",strInputFilename.c_str());
        }
    }

    try
    {
        x=atoi(std::string(argv[3]).c_str());
        y=atoi(std::string(argv[4]).c_str());
        offset_x=atoi(std::string(argv[5]).c_str());
        offset_y=atoi(std::string(argv[6]).c_str());
    }
    catch(std::exception& e)
    {
        ;//fprintf(stderr, "[**** ERROR] %s\n",e.what());
    }

    if(strInputRunname.empty())
    {
        dir="./../IMG";
        bIsDir=true;
    }
    else
    {
        if(boost::filesystem::exists(dir))          //判断文件是否存在
        {
            if(boost::filesystem::is_directory(dir)) //文件存在，并且是目录
            {
                bIsDir=true;
            }
            else                                    //存在，但不是目录
            {
                bIsDir=false;
            }
        }
        else                                        //文件不存在， 使用默认目录
        {
            dir="./../IMG";
            bIsDir=true;
        }
    }




    fprintf(stdout, ">>>>>  RUN: %s\n",dir.string().c_str());
    fprintf(stdout, ">>>>>  Offset:left=%d,top=%d,bottom=%d,right=%d \n",x,y,offset_x,offset_y);

    size_t shm_size(_KEY_SIZE_);
    key_t key_998(_KEY_T_);
    void* shared_memory(0);
    SHARE_MEMORY_ST* shm_buffer(0);
    int shmid=shmget(key_998,(size_t)shm_size,0666|IPC_CREAT);
    if(-1==shmid)
    {
        cout<<__FILE__<<"\t"<<__LINE__<<endl;
        perror("[create share memory error:]");
    }
    else
    {
        if((void*)(-1)!=(shared_memory=shmat(shmid,(void*)0,0)))
        {
            shm_buffer=(struct SHARE_MEMORY_ST*)shared_memory;
            //memset(shm_buffer,'\0',sizeof(struct SHARE_MEMORY_ST));
            shm_buffer->nShm_Memory_Status=0;
        }
    }


    std::vector<boost::filesystem::path> vPath;
    if(bReadInputFile)                                                  //需要读取文件，按行
    {
        std::ifstream fin(strInputFilename.c_str(),std::ios::in);
        char line_buffer[1024*8];
        memset(line_buffer,0x00,sizeof(line_buffer));
        std::vector<std::string> strContainer;
        while(fin.getline(line_buffer,sizeof(line_buffer)))
        {
            std::string strFileLine;
            std::stringstream word(line_buffer);
            word>>strFileLine;
            //std::cout<<strFileLine.c_str()<<std::endl;
            strContainer.push_back(strFileLine);
        }
        fin.clear();
        fin.close();
        for(std::vector<std::string>::iterator iter(strContainer.begin());iter!=strContainer.end();++iter)  //逐行解析，方式 CSV
        {
            boost::escaped_list_separator<char> sep;
            tokenizer< boost::escaped_list_separator<char> > tok(*iter,sep);
            for(BOOST_AUTO(pos,tok.begin());pos!=tok.end();++pos)
            {
                vPath.push_back(boost::filesystem::path(*pos));
                //std::cout<<*pos<<std::endl;
            }
        }
    }

    vector<path> tmp;
    string filename = "*.bmp";
    char* wnd_name = (char*)"show_image";  
    bool bSort(false);          //标记是否已经排序
    bool bAppend(false);
    int nBase(tmp.size());    


    if(bIsDir)                                                          //如果是目录
    {
        find_files(dir,filename,tmp);
    }
    else                                                                //文件配置
    {
        bSort=true;                                                     //如果是读取文件配置，则不再排序
        std::ifstream fin(dir.string().c_str(),std::ios::in);
        char line_buffer[1024*8];
        memset(line_buffer,0x00,sizeof(line_buffer));
        std::vector<std::string> strContainer;
        while(fin.getline(line_buffer,sizeof(line_buffer)))
        {
            std::string strFileLine;
            std::stringstream word(line_buffer);
            word>>strFileLine;
            //std::cout<<strFileLine.c_str()<<std::endl;
            strContainer.push_back(strFileLine);
        }
        fin.clear();
        fin.close();
        for(std::vector<std::string>::iterator iter(strContainer.begin());iter!=strContainer.end();++iter) //逐行解析，方式 CSV
        {
            boost::escaped_list_separator<char> sep;                                    //按照CSV文件进行解析
            tokenizer< boost::escaped_list_separator<char> > tok(*iter,sep);            //结果存入tok
            for(BOOST_AUTO(pos,tok.begin());pos!=tok.end();++pos)                       //读取解析结果,类型未知
            {
                tmp.push_back(boost::filesystem::path(*pos));
                //std::cout<<*pos<<std::endl;
            }
        }
    }


    shm_buffer->nShm_Memory_Status=0;

    while(true)
    {
        if(nBase>0 && false==bSort)
        {
            sort(tmp.begin(),tmp.end(),SortFunc);
            bSort=true;
            //random_shuffle(tmp.begin(),tmp.end());
        }

        if(vPath.size()>0 && false==bAppend)
        {
            //tmp.reserve(vPath.size()+tem.size());
            /*for(std::vector<boost::filesystem::path>::const_iterator con_iter(vPath.begin());con_iter!=vPath.end();--con_iter)
              {
              tmp.push_back(*con_iter);
              }
              */
            for(std::vector<boost::filesystem::path>::const_iterator con_iter(tmp.begin());con_iter!=tmp.end();++con_iter)
            {
                vPath.push_back(*con_iter);
            }
            tmp.clear();
            tmp.assign(vPath.begin(),vPath.end());
            bAppend=true;
        }


        for(std::vector<boost::filesystem::path>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            cout <<"\nFileName:" <<*it <<endl;
            if(!boost::filesystem::exists(*it))
            {
                fprintf(stderr, "[**** ERROR] %s\t[filename: %s ]\n","File Not Exists !",it->string().c_str());
                continue;
            }
            //char* str=(char*)it->string().c_str();
            try
            {

                //char* file_name = argc == 2 ? argv[1] : (char*)"1.bmp" ; 

                //char* lpszChar=(char*)"9999-123-31-23-59-59-000000000000000.bmp";
                //ptime p1=microsec_clock::local_time();
                //char strName[_DEF_FILENAME_LEN_];
                //memset(strName,0x00,sizeof(strName));
                //date d=p1.date();
                //sprintf(strName,"%d-%d-%d-%d-%d-%d-%lld",(int)d.year(),(int)d.month(),(int)d.day(),p1.time_of_day().hours(),p1.time_of_day().minutes(),p1.time_of_day().seconds(),p1.time_of_day().ticks()); 
                char* str=(char*)it->string().c_str();
                string str_filename(it->filename().c_str());
                if(0==isdigit(str_filename[0]))
                {
                    fprintf(stdout, "%s\n","File Name Not Start With Number");
                    continue;
                }
                //IplImage* src = cvLoadImage(str,1); 
                Mat src1=imread(str, 1);
                //int offset=350;
                int offset=x;//error offset
                Rect NewRect = Rect(0, 0, offset, src1.rows);
                //Rect NewRect=Rect(x,y,offset_x,offset_y);

                Mat srctemp(src1, NewRect);
                srctemp = Scalar(0);

                IplImage src2 = src1;
                IplImage *src = &src2;

                // src2 = srctemp.clone();

                // cout <<"1" << src1.cols*src1.rows*3<<endl;
                // IplImage src3= src2;
                // cout <<"1" << src3.width << "::"<< src3.widthStep << endl;
                // IplImage *src = &src3;

                // src->imageSize = src->width*src->height*3;

                // cout <<"1" <<src->width <<":" << src->imageSize<<  endl;

                switch(shm_buffer->nShm_Memory_Status)
                {
                    case 0:
                        {
                            shm_buffer->nShm_Memory_Status=3;
                            if(0==shm_buffer->nShm_Memory_ReadStatus_0)
                            {
                                cout<<__FILE__<<__LINE__<<"\t Part 1 Is Not Read ,Write Again"<<endl;
                            }
                            shm_buffer->iPixelType_0           =0;
                            shm_buffer->iSizeX_0               =src->width;
                            shm_buffer->iSizeY_0               =src->height;
                            shm_buffer->iImageSize_0           =src->imageSize;

                            cout<<"width :"<<shm_buffer->iSizeX_0<<endl;
                            cout<<"height:"<<shm_buffer->iSizeY_0<<endl;
                            cout<<"size  :"<<shm_buffer->iImageSize_0<<endl;

                            shm_buffer->iTimeStamp_0           =0;
                            shm_buffer->iMissingPackets_0      =0;
                            shm_buffer->iAnnouncedBuffers_0    =0;
                            shm_buffer->iQueuedBuffers_0      =0;
                            shm_buffer->iOffsetX_0             =0;
                            shm_buffer->iOffsetY_0             =0;
                            shm_buffer->iAwaitDelivery_0       =0;
                            shm_buffer->iBlockId_0             =0;
                            memset(shm_buffer->lpszImageBuffer_0,'\0',sizeof(shm_buffer->lpszImageBuffer_0));
                            memcpy(shm_buffer->lpszImageBuffer_0,src->imageData,src->imageSize);

                            //cout << "2" << endl;
                            string strFileName(it->filename().c_str());
                            memset(shm_buffer->lpszFileName_0,0x00, strlen(shm_buffer->lpszFileName_0));
                            memcpy(shm_buffer->lpszFileName_0,strFileName.c_str(), strlen(strFileName.c_str()));
                            //cout << "2" << endl;
                            //memcpy(shm_buffer->lpszFileName_0,strName,sizeof(strName));
                            //J_Image_SaveFileEx((J_tIMAGE_INFO*)this->shm_buffer->szShm_Memory_Alloc_0, "capture_0.bmp", J_FF_BMP, 100);  // Stores to a JPEG file.
                            //J_Image_SaveFileEx(&BufferInfo, "capture_0.jpg", J_FF_JPEG, 75);
                            shm_buffer->nShm_Memory_ReadStatus_0=0;
                            shm_buffer->nShm_Memory_Status=1;
                            //cout << "2" << endl;
                        }
                        break;
                    case 1:
                        {
                            shm_buffer->nShm_Memory_Status=4;
                            if(0==shm_buffer->nShm_Memory_ReadStatus_1)
                            {
                                cout<<__FILE__<<__LINE__<<"\t Part 1 Is Not Read ,Write Again"<<endl;
                            }
                            shm_buffer->iPixelType_1           =0;
                            shm_buffer->iSizeX_1               =src->width;
                            shm_buffer->iSizeY_1               =src->height;
                            shm_buffer->iImageSize_1           =src->imageSize;

                            cout<<"width :"<<shm_buffer->iSizeX_1<<endl;
                            cout<<"height:"<<shm_buffer->iSizeY_1<<endl;
                            cout<<"size  :"<<shm_buffer->iImageSize_1<<endl;

                            shm_buffer->iTimeStamp_1           =0;
                            shm_buffer->iMissingPackets_1      =0;
                            shm_buffer->iAnnouncedBuffers_1    =0;
                            shm_buffer->iQueuedBuffers_1      =0;
                            shm_buffer->iOffsetX_1             =0;
                            shm_buffer->iOffsetY_1             =0;
                            shm_buffer->iAwaitDelivery_1       =0;
                            shm_buffer->iBlockId_1             =0;
                            memset(shm_buffer->lpszImageBuffer_1,'\0',sizeof(shm_buffer->lpszImageBuffer_1));
                            memcpy(shm_buffer->lpszImageBuffer_1,src->imageData,src->imageSize);
                            string strFileName(it->filename().c_str());
                            memset(shm_buffer->lpszFileName_1,0x00, strlen(shm_buffer->lpszFileName_1));
                            memcpy(shm_buffer->lpszFileName_1,strFileName.c_str(), strlen(strFileName.c_str()));
                            //memcpy(shm_buffer->lpszFileName_1,strName,sizeof(strName));
                            shm_buffer->nShm_Memory_ReadStatus_1=0;
                            shm_buffer->nShm_Memory_Status=0;
                        }
                        break;
                    default:
                        {
                            cout<<__FILE__<<__LINE__<<"\tImage Acquisition Too Fast..."<<endl;
                        }
                        break;
                }

                cvNamedWindow(wnd_name,0);  
                cvShowImage(wnd_name,src);  
                cvWaitKey(1000);
                //cvReleaseImage(&src);  
            }
            catch(std::exception& e)
            {
                cout<<e.what()<<endl;
            }
        }
    }

    cvDestroyWindow(wnd_name);
    if(shmid!=-1)
    {
        if(shmdt(shared_memory)==-1)
        {
            cout<<__FILE__<<"\t"<<__LINE__<<endl;
        }
        else
        {
            if(shmctl(shmid,IPC_RMID,0)==-1)
            {
                cout<<__FILE__<<"\t"<<__LINE__<<endl;
            }
        }
    }

}
