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
  
#include <SDL/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//==========
#include <sys/stat.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
//==========

#include <signal.h>
#define _GNU_SOURCE
#include <getopt.h>

#ifdef _MSC_VER
#include <io.h>     /* _setmode() */
#include <fcntl.h>  /* _O_BINARY */
#endif

#ifndef _MSC_VER
#include "config.h"
#endif

#include "common/common.h"
#include "x264.h"
#include "muxers.h"

#define DATA_MAX 3000000
uint8_t data[DATA_MAX];


#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64

//#define DATA_SIZE 3000000
//uint8_t data[DATA_SIZE]
//#include "avcodec.h"

/* here we can use OS dependant allocation functions */
#undef malloc
#undef free
#undef realloc

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif


//#define INT_MAX 2147483647

////////////////////////////////////////////////////////////////
#define HAVE_AV_CONFIG_H
#include "ffmpeg/avformat.h"
#include "cmdutils.h"
////////////////////////////////////////////////////////////////

//指向图像的指针
typedef struct{
    long int now;
    char data[1];
}VideoFrame;

VideoFrame* pic=NULL;

int PictureWidth = 704;
int PictureHeight = 576;
int BufForImage = 6;
char* yDecImage=NULL, *vDecImage=NULL, *uDecImage=NULL;

int Image_handle = -1;

/* Ctrl-C handler */
static int     b_ctrl_c = 0;

static void    SigIntHandler( int a )
{
    b_ctrl_c = 1;
}

int video_frame_rate;
int video_frame_rate_base;
int img_width;
int img_height;
int video_bit_rate;
int frame_count = 0;

typedef struct {
    int b_decompress;
    int b_progress;
    int i_seek;
    hnd_t hin;
    hnd_t hout;
} cli_opt_t;



/* input file operation function pointers */
int (*p_open_infile)( char *psz_filename, hnd_t *p_handle, x264_param_t *p_param );
int (*p_get_frame_total)( hnd_t handle );
int (*p_read_frame)( x264_picture_t *p_pic, hnd_t handle, int i_frame );
int (*p_close_infile)( hnd_t handle );

/* output file operation function pointers */
static int (*p_open_outfile)( char *psz_filename, hnd_t *p_handle );
static int (*p_set_outfile_param)( hnd_t handle, x264_param_t *p_param );
static int (*p_write_nalu)( hnd_t handle, uint8_t *p_nal, int i_size );
static int (*p_set_eop)( hnd_t handle, x264_picture_t *p_picture );
static int (*p_close_outfile)( hnd_t handle );

static int  Parse_main(cli_opt_t *opt, char *filename );
static void my_param( x264_param_t *param );

static const char * const overscan_str[] = { "undef", "show", "crop", NULL };
static const char * const vidformat_str[] = { "component", "pal", "ntsc", "secam", "mac", "undef", NULL };
static const char * const fullrange_str[] = { "off", "on", NULL };
static const char * const colorprim_str[] = { "", "bt709", "undef", "", "bt470m", "bt470bg", "smpte170m", "smpte240m", "film", NULL };
static const char * const transfer_str[] = { "", "bt709", "undef", "", "bt470m", "bt470bg", "smpte170m", "smpte240m", "linear", "log100", "log316", NULL };
static const char * const colmatrix_str[] = { "GBR", "bt709", "undef", "", "fcc", "bt470bg", "smpte170m", "smpte240m", "YCgCo", NULL };


/****************************************************************************
 * my_param:
 ****************************************************************************/
void    my_param( x264_param_t *param )
{
    
    /* Video properties */
    param->i_width         = 176;
    param->i_height        = 144;
    param->i_fps_num       = 25;
    param->i_fps_den       = 1;
    
    /* Encoder parameters */
    param->i_frame_reference = 3;
    param->i_keyint_max = 1;
    param->i_keyint_min = 1;
    
    param->b_cabac = 0;
    
//    param->rc.b_cbr = 1;
    param->rc.i_rc_method = X264_RC_ABR;
    param->rc.i_bitrate = 3000;
    
    /* */
    //param->analyse.intra = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8;
    param->analyse.inter = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8
                         | X264_ANALYSE_PSUB16x16 | X264_ANALYSE_BSUB16x16 | X264_ANALYSE_PSUB8x8;
    param->analyse.i_subpel_refine = 4;
    param->analyse.b_chroma_me = 0;
    param->analyse.b_psnr = 0;
    
    param->i_frame_total = 0;
}

/* add a video output stream */
//在此函数中修改转码后的帧率、码率、分辨率!!!!!!!!!!!!!!!!!!!!!!!!!
void update_param(x264_param_t *param)
{
    /* resolution must be a multiple of two */
    param->i_width = img_width;
    param->i_height = img_height;
    /* frames per second */
    param->i_fps_den = video_frame_rate;
    param->i_fps_num = video_frame_rate_base;
    
    param->rc.i_bitrate = video_bit_rate/1000 ;
    printf("bit rate : %d!\n", param->rc.i_bitrate);
    
    param->i_keyint_max = param->i_keyint_min = 25; //100;
    
    param->i_log_level=X264_LOG_NONE;
    
    param->analyse.i_me_method = 0;
    param->analyse.i_subpel_refine = 1; 
    param->analyse.intra = 1;
    param->analyse.inter =/* X264_ANALYSE_I4x4 |*/ X264_ANALYSE_I8x8
                        /* | X264_ANALYSE_PSUB16x16*/;
                         
    param->i_frame_reference = 1;
    	 
}
    
/*****************************************************************************
 * Parse_main:
 *****************************************************************************/
static int  Parse_main(cli_opt_t *opt, char *filename )
{
    
    memset( opt, 0, sizeof(cli_opt_t) );

    /* Default output file driver */
    p_open_outfile = open_file_bsf;
    p_set_outfile_param = set_param_bsf;
    p_write_nalu = write_nalu_bsf;
    p_set_eop = set_eop_bsf;
    p_close_outfile = close_file_bsf;

    /* Parse command line options */
    opterr = 0; // no error message
    
    optarg = filename;
    
    if( p_open_outfile( optarg, &opt->hout ) )
          {
    	fprintf( stderr, "cannot open output file `%s'\n", optarg );
      return -1;
           }
           
    
    return 0;
}
/*****************************************************************************
 * Encode:
 *****************************************************************************/

static int  Encode_frame( AVFormatContext *oc, x264_t *h, hnd_t hout, x264_picture_t *pict )
{
    x264_picture_t pic_out;
    x264_nal_t *nal;
    int i_nal, i;
    int i_file = 0;
    static int pts; 
    
    AVPacket pkt;
    /* Do not force any parameters */
    if( pict )
    {
        pict->i_type = X264_TYPE_AUTO;
        pict->i_qpplus1 = 0;
    }
    if( x264_encoder_encode( h, &nal, &i_nal, pict, &pic_out ) < 0 )
    {
        fprintf( stderr, "x264_encoder_encode failed\n" );
    }
//printf("***********%d**********\n", i_nal);
    for( i = 0; i < i_nal; i++ )
    {
        int i_size;
        int i_data;
	 
		i_data = DATA_MAX;
        
	    av_init_packet(&pkt);
        if( ( i_size = x264_nal_encode( data, &i_data, 1, &nal[i] ) ) > 0 )
        {
			//printf("nal type: %d, %d, %d!\n", (&nal[i])->i_type, (&nal[i])->i_ref_idc, i_size);
			if( (&nal[i])->i_type == 0 ) {
				printf("NAL FAILED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
			}
			pkt.pts=av_rescale_q(pts++,oc->streams[0]->codec->time_base,oc->streams[0]->time_base);
		//	printf("pts=%lld\n",pkt.pts);
        	pkt.stream_index = 0; 
    		pkt.data = data;
			pkt.size = i_size;
			//printf("%d \n",i_size);
			if(av_write_frame(oc, &pkt)!=0){
		    	fprintf(stderr, "Error while writing video frame\n");
			}	
        }
        else if( i_size < 0 )
        {
            fprintf( stderr, "need to increase buffer size (size=%d)\n", -i_size );
        }
    }
    return i_file;
}



void PicData(VideoFrame* pic)

{
    static int emptytime = 0;
    static long int framegetCount = 2;
    
//    static VideoPacket vpkt;

    //fprintf(stderr, "The PicData Processing, the Pic->now is %d, framegetCount is %d\n", pic->now, framegetCount);
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
	    framegetCount ++;
	    
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

void DecImageInit( char *pshmName )
{
    
    char createFile[100] = "touch ";
    pstrcat(createFile, 50 , pshmName);
    
    FILE *f = popen(createFile, "w");
    pclose(f);

    key_t Image_key=ftok(pshmName, 'S');
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

void on_open_stream(FILE *f,char *stream)
{
    /* open the file */
    //char *msg = malloc(1024); 
    char msg[1024] =""; 
    snprintf(msg,1024,"OpenStream MULTICAST:%s\n",stream);
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

FILE* on_proc(char *pshmName, char *psourcetype)
{
    FILE *f;
    /* start the plugins */
    char *cmd =(char*) av_malloc(1024);
    //char cmd[1024] = ""; 
    //getlocalip();

    //sprintf(cmd, "./dec_hik_dm642_d1 %s %s", pshmName, LocalIP);
    snprintf(cmd, 1024,"./dec_%s %s %s", psourcetype, pshmName, LocalIP);
     
    f = popen(cmd,"w");
    printf("Cmd is %s\n", cmd);

    av_free(cmd);
    return f;
}



void usage( void )
{
	fprintf( stderr, "Usage:\r\n" );
	fprintf( stderr, "\th264_ts <source type> <Multicast address>:<Multicast port> <sharememory> <output stream>\r\n" );
	fprintf( stderr, "<source type>                           like mpeg2 hik_m4 hik_d1 hik_cif\r\n" );
	fprintf( stderr, "<Multicast address>:<Multicast port>    like 224.2.213.158:10058\r\n" );
	fprintf( stderr, "<sharememory>                           like .libotest.shm\r\n" );
	fprintf( stderr, "<output stream>                         like udp://192.168.100.74:5001 or udp://224.0.0.1:5001?multicast\r\n" );
}

int main(int argc, char** argv)
{
    if(argc < 6)
	{	usage();
		return -1;
	}
	av_register_all();	
    char outfilename[100];
    char sourcetype[50];  
    char shmName[50];
    char ipport[1024];
    snprintf(LocalIP,48,"%s",argv[5]);
    snprintf(sourcetype, 50, "%s", argv[1]);
	snprintf(shmName,50,"%s",argv[3]);
    snprintf(ipport,1024, "%s",argv[4]);
	if( strncmp(sourcetype, "hik_m4", 5)==0 || strncmp(sourcetype, "hik_cif", 7)==0 )
	{	
		PictureWidth = PictureWidth / 2;
		PictureHeight = PictureHeight / 2;
		video_bit_rate =250000; 
	}
	else
	 	video_bit_rate = 1000000;
	fprintf( stderr, "picture = %d X %d\r\n", PictureWidth, PictureHeight );
//////////////////////////////////////////////////////////////////////////////
    AVFormatContext  oc1, *oc=&oc1;
    AVStream video_st1, *video_st = &video_st1;
    AVFormatParameters video_parmas, *video_ap=&video_parmas;
    int i, video_index;
    video_index = -1;
    memset(video_ap, 0 , sizeof(*video_ap));


//////////////////////////////////////////////////////////////////////////////

    x264_param_t param;
    cli_opt_t opt;
    
    x264_t *h;
    x264_picture_t pict;
 
    

    int out_size;

    DecImageInit( shmName );
   
    x264_param_default( &param );
    
    my_param( &param );    

    memcpy(outfilename,"test.264",sizeof(outfilename));

         /* Parse command line */
    if( Parse_main(&opt, (char *)outfilename ) < 0 )
        return -1;	
	
    img_width = PictureWidth;
	img_height = PictureHeight;
	
	video_frame_rate = 1;
	video_frame_rate_base =25;
	
	update_param(&param);

  	if( ( h = x264_encoder_open( &param ) ) == NULL )
    {
        fprintf( stderr, "x264_encoder_open failed\n" );
        p_close_infile( opt.hin );
        p_close_outfile( opt.hout );
        return -1;
    }
     /* Create a new pic */
    x264_picture_alloc( &pict, X264_CSP_I420, param.i_width, param.i_height );


    //开始播放图像
    FILE* decoding = on_proc( shmName, sourcetype );
    
    on_open_stream(decoding, argv[2]);
	
    printf("Start to process\n");
    sleep(1);

   	signal(SIGINT,SigIntHandler);

 

//////////////////////////////////////////////////////////////////////////////

    AVCodecContext *c;
    oc = av_alloc_format_context();
    oc->oformat = guess_format(NULL,".h264",NULL);


	printf("nv_streams is %d\n",oc->nb_streams);
    video_st = av_new_stream(oc,0);
    c = video_st->codec;
    c->codec_id = CODEC_ID_H264;
    c->codec_type = CODEC_TYPE_VIDEO;

    c->width = img_width;
    c->height = img_height;
    /* time base: this is the fundamental unit of time (in seconds) in terms
       of which frame timestamps are represented. for fixed-fps content,
       timebase should be 1/framerate and timestamp increments should be
       identically 1. */
    c->time_base.den = 25;
    c->time_base.num = 1;
    c->gop_size = 1; /* emit one intra frame every twelve frames at most */
    c->pix_fmt = PIX_FMT_YUV420P;
    
    av_set_parameters(oc, NULL);
    if(!(oc->oformat->flags & AVFMT_NOFILE))
    {
	if (url_fopen(&oc->pb, ipport, URL_WRONLY ) <0 )
	{
	    fprintf(stderr, "Could not open '%s'\n", ipport);
	    exit(1);
	}
    }

    av_write_header(oc);
//////////////////////////////////////////////////////////////////////////////

   while(!b_ctrl_c)
    {
       
		PicData(pic);

       	pict.img.plane[0]=yDecImage;
       	pict.img.plane[1]=uDecImage;  
       	pict.img.plane[2]=vDecImage;

	
		pict.i_pts = (int64_t)frame_count * param.i_fps_den;
					
		out_size = Encode_frame( oc ,h, opt.hout, &pict );				
		//		printf(" output_size: %d.\n", out_size);			
		frame_count++;

    }  
    
    x264_picture_clean( &pict );
    
    x264_encoder_close( h );

//////////////////////////////////////////////////////////////////////////////  
    av_write_trailer(oc);
    for(i = 0;i < oc->nb_streams; i++)
    {
        av_freep(&oc->streams[i]);
    }
    if(!oc->oformat->flags & AVFMT_NOFILE)
	url_fclose(&oc->pb);
    av_free(oc);

//////////////////////////////////////////////////////////////////////////////


    p_close_outfile( opt.hout );

    return 0;

}
