#include <iostream>
#include <fstream>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <errno.h>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/tokenizer.h>
#include <google/protobuf/compiler/parser.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include "Camera.pb.h"

#include "colorsep.h"

using namespace std;
using namespace cv;

// int imgresult=-1;
// int refresult=-1;


// int sockfd;
// int Refsockfd;

struct ImageSize
{
    int isize;
    /* data */
};
struct RefNum
{
    int rnum;
    char refseq[100];
};

int InitImageTcp(int &Imgsockfd)
{
    cout <<"Init sendImage" <<endl;
    //初始化一个TCP客户端

    int len;
    struct sockaddr_in address; 
    //char ch='A';
    Imgsockfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=inet_addr("127.0.0.1");
    address.sin_port=htons(11012);
    len=sizeof(address);
    //bind(Imgsockfd,(struct sockaddr*)&address,len);
    int imgresult=connect(Imgsockfd,(struct sockaddr*)&address,len);

    if(imgresult==-1)
    {
        perror("oops :client1");
        exit(1);
    }

    return imgresult;
}

int sendImage(DataForDisp* data, int &Imgsockfd)//接收一个结构体
{

    //result=-1;
    //
    // if (imgresult==-1)
    // {
    // 	imgresult=InitImageTcp(); 
    // }

    TestPB::Mat tmat;
    cout<<"Loading..."<<endl;
    //将结构体中的数据赋值给protobuf
    cout << "My Send data is " << data->types << ":::" << data->sequence 
        << ":::" << data->typessequence << endl;

    tmat.set_types(data->types);
    tmat.set_sequence(data->sequence);
    tmat.set_typessequence(data->typessequence);
    tmat.set_collapse(data->collapse);
    cout << "1" << endl;
    imwrite("wbimage.bmp", data->DispImage);
    //cout << "CurDisp.DispImage " << data->DispImage.cols << ", " << data->DispImage.rows << endl;
    tmat.set_data(data->DispImage.data,data->DispImage.rows*data->DispImage.step);//图片的data数据

    cout << "1" << endl;
    //vector<float> debugData(8,4.0);
    for (vector<float>::iterator it=data->debugData.begin();it!=data->debugData.end();++it)
    {	//debugdata->set_debugdata()
        tmat.add_debugdata(*it);
    }
    //FILE *outfile;
    string imgstr="";
    //outfile = fopen("sv.conf","wb");
    tmat.SerializeToString(&imgstr);
    //fwrite(imgstr,21689584);
    /*
       string imgstr;
       fstream output(argv[1], ios::out | ios::trunc | ios::binary);
       if (!tmat.SerializeToOstream(&output)) {
       cerr << "Failed to write address book." << endl;
       return -1;
       }
       */

    //string imagename = "1.bmp";
    char send_sock_parameter[imgstr.length()];
    for(int i=0;i<imgstr.length();i++)
    {
        send_sock_parameter[i] = imgstr[i];
    }
    //send_sock_parameter[imgstr.length()] = '\0';
    cout<<sizeof(send_sock_parameter)<<endl;
    ImageSize imgsize={sizeof(send_sock_parameter)};
    write(Imgsockfd,&imgsize,sizeof(imgsize));
    //mdata=send_sock_parameter.data;
    //int send_sock_parameter='A';
    //cout<<mdata<<endl;
    //memcpy(send_sock_parameter,);
    //      write(Imgsockfd,send_sock_parameter,sizeof(send_sock_parameter));
    int rs=send(Imgsockfd,send_sock_parameter,sizeof(send_sock_parameter),0);
    //      printf("_______________________________________________________________________________________IMG sended %d! \n",rs);
    if(rs<=0)
    {
        Imgsockfd=0;
        printf("_______________________________________________________________________________________IMGserver close!\n");
    }
    //close(Imgsockfd);

    /*
       if (!tmat.SerializeToString(&imgstr)) {
       cerr << "Failed to write address book." << endl;
       return -1;
       }
       output.write((char*)imgstr.c_str(), 121651);
       */
    /*
       {
    // Write the new address book back to disk.
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!mat.SerializeToOstream(&output)) {
    cerr << "Failed to write address book." << endl;
    return -1;
    }
    }
    */
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    cout << "my send Data done\n" << endl;
    return 0;
}

int InitRefTcp(int &Refsockfd)
{
    cout<<"Init RefTcp"<<endl;
    int len;
    struct sockaddr_in address; 
    //char ch='A';
    Refsockfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=inet_addr("127.0.0.1");
    address.sin_port=htons(11013);
    len=sizeof(address);
    //bind(sockfd,(struct sockaddr*)&address,len);
    int refresult=connect(Refsockfd,(struct sockaddr*)&address,len);

    if(refresult==-1)
    {
        perror("oops :client1");
        exit(1);
    }
    return refresult;
} 

int sendInitRefImage(int i,char *refseq, int Refsockfd)
{
    // if(refresult==-1)
    //  {
    //     refresult=InitRefTcp();
    //  }
    RefNum refnum;
    refnum.rnum=i;
    strncpy(refnum.refseq,refseq,sizeof(refnum));
    //int send_sock_parameter = i;
    write(Refsockfd,&refnum,sizeof(refnum));
    //close(sockfd);
    return 0;
}


int sendNewRefImage(int i, int Refsockfd)
{
    cout << "sendNewRefImage at Refsockfd " << Refsockfd << endl;
    // if(refresult==-1)
    //  {
    //     refresult=InitRefTcp();
    //  }
    RefNum refnum;
    refnum.rnum=i;
    memset(refnum.refseq,'\0',100);
    //bzero(refnum.refseq,100);
    cout << "sendNewRefImage at Refsockfd " << Refsockfd << "with size " << sizeof(refnum) <<endl;
    cout << "data.rnum " << refnum.rnum << endl;
    cout << "data.refseq " << refnum.refseq << endl;
    //strncpy(refnum.refseq,refseq,sizeof(refnum));
    //int send_sock_parameter = i;
    //int wbyte =
    write(Refsockfd,&refnum,sizeof(refnum));
    //close(sockfd);
    cout << "sendNewRefImage" << endl;
    return 0;
}
