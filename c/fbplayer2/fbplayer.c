/*
 * =====================================================================================
 *
 *       Filename:  fbplayer.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  三月 14, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>

char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
// font size
int width = 10;
int height = 10;
// font position
int pos_x = 10;
int pos_y = 100;

struct font_color {
    int r;
    int g;
    int b;
    int a;
};
typedef struct font_color FONT_COLOR;

void drawdot(int x, int y, FONT_COLOR fc)
{
    long int location = 0;
    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
        (y+vinfo.yoffset) * finfo.line_length;


    *(fbp + location) = fc.r;	        /* Some blue */
    *(fbp + location + 1) = fc.g;       /* A little green */
    *(fbp + location + 2) = fc.b;       /* A lot of red */
    *(fbp + location + 3) = fc.a;       /* No transparency */
}

void drawdigit(int pos_x, int pos_y, int num, FONT_COLOR fc)
{

    int i = 0;
    switch (num) {
        case 0:
            // draw horizontal
            for (i=0; i<width; i++) {
                drawdot(pos_x+i, pos_y, fc);
                drawdot(pos_x+i, pos_y+2*height, fc);
            }
            // draw vertical
            for (i=0; i<height; i++) {
                drawdot(pos_x, pos_y+i, fc);
                drawdot(pos_x+width, pos_y+i, fc);
                drawdot(pos_x, pos_y+height+i, fc);
                drawdot(pos_x+width, pos_y+height+i, fc);
            }
            break;
        case 1:
            // draw vertical
            for (i=0; i<height; i++) {
                drawdot(pos_x, pos_y+i, fc);
                drawdot(pos_x, pos_y+height+i, fc);
            }
            break;
        case 2:
            // draw horizontal
            for (i=0; i<width; i++) {
                drawdot(pos_x+i, pos_y, fc);
                drawdot(pos_x+i, pos_y+height, fc);
                drawdot(pos_x+i, pos_y+2*height, fc);
            }
            // draw vertical
            for (i=0; i<height; i++) {
                drawdot(pos_x+width, pos_y+i, fc);
                drawdot(pos_x, pos_y+height+i, fc);
            }
            break;
        case 3:
            // draw horizontal
            for (i=0; i<width; i++) {
                drawdot(pos_x+i, pos_y, fc);
                drawdot(pos_x+i, pos_y+height, fc);
                drawdot(pos_x+i, pos_y+2*height, fc);
            }
            // draw vertical
            for (i=0; i<height; i++) {
                drawdot(pos_x+width, pos_y+i, fc);
                drawdot(pos_x+width, pos_y+height+i, fc);
            }
            break;
        case 4:
            // draw horizontal
            for (i=0; i<width; i++) {
                drawdot(pos_x+i, pos_y+height, fc);
            }
            // draw vertical
            for (i=0; i<height; i++) {
                drawdot(pos_x, pos_y+i, fc);
                drawdot(pos_x+width, pos_y+i, fc);
                drawdot(pos_x+width, pos_y+height+i, fc);
            }
            break;
        case 5:
            // draw horizontal
            for (i=0; i<width; i++) {
                drawdot(pos_x+i, pos_y, fc);
                drawdot(pos_x+i, pos_y+height, fc);
                drawdot(pos_x+i, pos_y+2*height, fc);
            }
            // draw vertical
            for (i=0; i<height; i++) {
                drawdot(pos_x, pos_y+i, fc);
                drawdot(pos_x+width, pos_y+i, fc);
                drawdot(pos_x, pos_y+height+i, fc);
                drawdot(pos_x+width, pos_y+height+i, fc);
            }
            break;
        case 6:
            // draw horizontal
            for (i=0; i<width; i++) {
                drawdot(pos_x+i, pos_y, fc);
                drawdot(pos_x+i, pos_y+height, fc);
                drawdot(pos_x+i, pos_y+2*height, fc);
            }
            // draw vertical
            for (i=0; i<height; i++) {
                drawdot(pos_x, pos_y+i, fc);
                drawdot(pos_x, pos_y+height+i, fc);
                drawdot(pos_x+width, pos_y+height+i, fc);
            }
            break;
        case 7:
            // draw horizontal
            for (i=0; i<width; i++) {
                drawdot(pos_x+i, pos_y, fc);
            }
            // draw vertical
            for (i=0; i<height; i++) {
                drawdot(pos_x, pos_y+i, fc);
                drawdot(pos_x+width, pos_y+i, fc);
                drawdot(pos_x+width, pos_y+height+i, fc);
            }
            break;
        case 8:
            // draw horizontal
            for (i=0; i<width; i++) {
                drawdot(pos_x+i, pos_y, fc);
                drawdot(pos_x+i, pos_y+height, fc);
                drawdot(pos_x+i, pos_y+2*height, fc);
            }
            // draw vertical
            for (i=0; i<height; i++) {
                drawdot(pos_x, pos_y+i, fc);
                drawdot(pos_x+width, pos_y+i, fc);
                drawdot(pos_x, pos_y+height+i, fc);
                drawdot(pos_x+width, pos_y+height+i, fc);
            }
            break;
        case 9:
            // draw horizontal
            for (i=0; i<width; i++) {
                drawdot(pos_x+i, pos_y, fc);
                drawdot(pos_x+i, pos_y+height, fc);
                drawdot(pos_x+i, pos_y+2*height, fc);
            }
            // draw vertical
            for (i=0; i<height; i++) {
                drawdot(pos_x, pos_y+i, fc);
                drawdot(pos_x+width, pos_y+i, fc);
                drawdot(pos_x+width, pos_y+height+i, fc);
            }
            break;
        case 100:
            // draw -
            for (i=0; i<width; i++) {
                drawdot(pos_x+i, pos_y+height, fc);
            }
            break;
        case 101:
            // draw :
            for (i=(height/4); i<(height/2); i++) {
                drawdot(pos_x+(width/2), pos_y+i, fc);
                drawdot(pos_x+(width/2), pos_y+height+i, fc);
            }
            break;
    }
}

void draw_date(int x, int y)
{
    struct timeval tv;
    struct tm *timetm;
    int nYear, nMon, nDay, nHour, nMin, nSec, nUsec;
    FONT_COLOR fc;
    if (gettimeofday(&tv, NULL) == 0) {
        timetm = localtime(&tv.tv_sec);

        nYear = timetm->tm_year + 1900;
        nMon = timetm->tm_mon + 1;
        nDay = timetm->tm_mday;
        nHour = timetm->tm_hour;
        nMin = timetm->tm_min;
        nSec = timetm->tm_sec;
        nUsec = tv.tv_usec;

        fc.r = 255; fc.g = 255; fc.b = 255; fc.a = 255;

        drawdigit(x, y, nYear/1000, fc);
        drawdigit(x+width+10, y, (nYear%1000)/100, fc);
        drawdigit(x+(width+10)*2, y, (nYear%1000%100)/10, fc);
        drawdigit(x+(width+10)*3, y, (nYear%1000%100%10), fc);

        drawdigit(x+(width+10)*4, y, 100, fc);
        drawdigit(x+(width+10)*5, y, (nMon/10), fc);
        drawdigit(x+(width+10)*6, y, (nMon%10), fc);

        drawdigit(x+(width+10)*7, y, 100, fc);
        drawdigit(x+(width+10)*8, y, (nDay/10), fc);
        drawdigit(x+(width+10)*9, y, (nDay%10), fc);

        drawdigit(x+(width+10)*11, y, (nHour/10), fc);
        drawdigit(x+(width+10)*12, y, (nHour%10), fc);

        drawdigit(x+(width+10)*13, y, 101, fc);
        drawdigit(x+(width+10)*14, y, (nMin/10), fc);
        drawdigit(x+(width+10)*15, y, (nMin%10), fc);

        drawdigit(x+(width+10)*16, y, 101, fc);
        drawdigit(x+(width+10)*17, y, (nSec/10), fc);
        drawdigit(x+(width+10)*18, y, (nSec%10), fc);

        drawdigit(x+(width+10)*19, y, 101, fc);
        drawdigit(x+(width+10)*20, y, (nUsec/100000), fc);
        drawdigit(x+(width+10)*21, y, (nUsec%100000/10000), fc);

        usleep(10000);

        fc.r = 0; fc.g = 0; fc.b = 0; fc.a = 0;

        drawdigit(x, y, nYear/1000, fc);
        drawdigit(x+width+10, y, (nYear%1000)/100, fc);
        drawdigit(x+(width+10)*2, y, (nYear%1000%100)/10, fc);
        drawdigit(x+(width+10)*3, y, (nYear%1000%100%10), fc);

        drawdigit(x+(width+10)*4, y, 100, fc);
        drawdigit(x+(width+10)*5, y, (nMon/10), fc);
        drawdigit(x+(width+10)*6, y, (nMon%10), fc);

        drawdigit(x+(width+10)*7, y, 100, fc);
        drawdigit(x+(width+10)*8, y, (nDay/10), fc);
        drawdigit(x+(width+10)*9, y, (nDay%10), fc);

        drawdigit(x+(width+10)*11, y, (nHour/10), fc);
        drawdigit(x+(width+10)*12, y, (nHour%10), fc);

        drawdigit(x+(width+10)*13, y, 101, fc);
        drawdigit(x+(width+10)*14, y, (nMin/10), fc);
        drawdigit(x+(width+10)*15, y, (nMin%10), fc);

        drawdigit(x+(width+10)*16, y, 101, fc);
        drawdigit(x+(width+10)*17, y, (nSec/10), fc);
        drawdigit(x+(width+10)*18, y, (nSec%10), fc);

        drawdigit(x+(width+10)*19, y, 101, fc);
        drawdigit(x+(width+10)*20, y, (nUsec/100000), fc);
        drawdigit(x+(width+10)*21, y, (nUsec%100000/10000), fc);
    }
}

int main()
{
    int fbfd = 0;
    long int screensize = 0;
    FONT_COLOR fc;

    /* Open the file for reading and writing */
    fbfd = open("/dev/fb0", O_RDWR);
    if (!fbfd) {
        printf("Error: cannot open framebuffer device.\n");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    /* Get fixed screen information */
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        printf("Error reading fixed information.\n");
        exit(2);
    }

    /* Get variable screen information */
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        printf("Error reading variable information.\n");
        exit(3);
    }

    /* Figure out the size of the screen in bytes */
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    printf("width:%d, height:%d\n", vinfo.xres, vinfo.yres);

    /* Map the device to memory */
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
            fbfd, 0);       
    if ((int)fbp == -1) { printf("Error: failed to map framebuffer device to memory.\n"); exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    /* Figure out where in memory to put the pixel */
    /*     location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
     *         (y+vinfo.yoffset) * finfo.line_length;
     * 
     *     *(fbp + location) = 255;
     *     *(fbp + location + 1) = 255; 
     *     *(fbp + location + 2) = 255; 
     *     *(fbp + location + 3) = 0; 
     * 
     */


    while (1) {
        draw_date(pos_x, pos_y);
    }

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
