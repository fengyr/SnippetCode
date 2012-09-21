/*
 * EP7 E-ink EPD BMP Picture Test Program
 * Copyright 2011 Foxconn TMSBG-CDPG Inc.
 * Author : Jerry.K.Luo
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "../inc/mxcfb.h"
#include <string.h>
 
#include "bmp24.h"

#define TRUE 1
#define FALSE 0

#define TPASS 0
#define TFAIL -1

#define BUFFER_FB		0
#define BUFFER_OVERLAY	1

#define ALLOW_COLLISIONS	0
#define NO_COLLISIONS		1

#define  ALIGN_PIXEL_128(x)  ((x+ 127) & ~127)

#define CROSSHATCH_ALTERNATING  0
#define CROSSHATCH_COLUMNS_ROWS 1

#define WAVEFORM_MODE_INIT	0x0	/* Screen goes to white (clears) */
#define WAVEFORM_MODE_DU	0x1	/* Grey->white/grey->black */
#define WAVEFORM_MODE_GC16	0x2	/* High fidelity (flashing) */
#define WAVEFORM_MODE_GC4	0x3	/* Lower fidelity */
#define WAVEFORM_MODE_A2	0x4	/* Fast black/white animation */

static int fd_fb = 0;
static unsigned short *fbp;
static unsigned int marker_val = 1;
static struct fb_var_screeninfo screen_info;
//static int wf_mode;

static void update_to_display(int left, int top, int width, int height, int wave_mode, int update_mode,	int wait_for_complete, uint flags)
{
	struct mxcfb_update_data upd_data;
	int retval;
	
	upd_data.update_mode = update_mode;		//1:full
	upd_data.waveform_mode = wave_mode;		//GC16
	upd_data.update_region.left = left;		//0
	upd_data.update_region.width = width;	//800
	upd_data.update_region.top = top;		//0
	upd_data.update_region.height = height;	//600
	upd_data.temp = TEMP_USE_AMBIENT;		//0x1000
	upd_data.flags = flags;					//0

	switch(wave_mode){
		case 0:
			printf("in mode INIT\n");
			break;
		case 1:
			printf("in mode DU\n");
			break;
		case 2:
			printf("in mode GC16\n");
			break;
		case 3:
			printf("in mode EMPTY/GC4\n");
			break;
		case 4:
			printf("in mode A2\n");
			break;
		case 5:
			printf("in mode GL16\n");
			break;
		case 6:
			printf("in mode A2IN\n");
			break;
		case 7:
			printf("in mode A2OUT\n");
			break;
		default:
			break;
	}
	if (wait_for_complete) {
		/* Get unique marker value */
		upd_data.update_marker = marker_val++;
	} else {
		upd_data.update_marker = 0;
	}

	retval = ioctl(fd_fb, MXCFB_SEND_UPDATE, &upd_data);
	while (retval < 0) {
		/* We have limited memory available for updates, so wait and
		 * then try again after some updates have completed */
		sleep(1);
		retval = ioctl(fd_fb, MXCFB_SEND_UPDATE, &upd_data);
	}

/*
	if (WAVEFORM_MODE_AUTO == WAVEFORM_MODE_AUTO)
		printf("Waveform mode used = %d\n", upd_data.waveform_mode);
*/

	if (wait_for_complete) {
		/* Wait for update to complete */
		retval = ioctl(fd_fb, MXCFB_WAIT_FOR_UPDATE_COMPLETE, &upd_data.update_marker);
		if (retval < 0) {
			printf("bmp_test : Wait for update complete failed.  Error = 0x%x\n", retval);
		}
	}
}

static void copy_image_to_buffer(int left, int top, int width, int height, uint *img_ptr,
			uint target_buf, struct fb_var_screeninfo *screen_info)
{
	int i;
	__u32 *dest, *src;
	uint *fb_ptr;

	if (target_buf == BUFFER_FB)
		fb_ptr =  (uint *)fbp;
	else if (target_buf == BUFFER_OVERLAY)
		fb_ptr = (uint *)fbp +
			(screen_info->xres_virtual * ALIGN_PIXEL_128(screen_info->yres) *
			screen_info->bits_per_pixel / 8) / 4;
	else {
		printf("Invalid target buffer specified!\n");
		return;
	}

	if ((width > screen_info->xres) || (height > screen_info->yres)) {
		printf("Bad image dimensions!\n");
		return;
	}

	for (i = 0; i < height; i++) {
		dest = fb_ptr + ((i + top) * screen_info->xres_virtual + left) * 2 / 4;
		src = img_ptr + (i * width) * 2 /4;
		memcpy(dest, src, width * 2);
	}
}


int main(int argc, char **argv)
{
	int ret = 0;	
	char fb_dev[10] = "/dev/fb0";
	int fb_size = 0;
	int auto_update_mode;
	int scheme;
	int i;
	char file_name[64];
	FILE *fp_bmp = NULL;
	struct mxcfb_waveform_modes wv_modes;
	unsigned int *buf = NULL;

	int wf_mode;
	
	if (argc != 5)
	{
		printf("%s <update mode> <waveform mode> <flag> <name>.bmp\n", argv[0]);
		printf("\nParameters:\n");
		printf("<update mode>  -  <1> full_update, <0> partial update\n");
		printf("<waveform mode>  -  you can use the interger between 0~6\n");
		printf("               0:init mode 1:du mode 2:gc16 mode 3:empty mode/gc4 mode 4:a2 mode 5:a2in mode 6:a2out mode\n");
		printf("<flag> 0:NORMAL 2:MONOCHROME\n");
		printf("\nExample:\n");
		printf("%s 1 3 0 test.bmp\n", argv[0]);
		return -1;
	}
	
	if (atoi(argv[1]) != 0 && atoi(argv[1]) != 1)
	{
		printf("Wrong update mode.\n");
		return -1;
	}
	
	wf_mode = atoi(argv[2]);
	if(wf_mode < 0 || wf_mode > 7){
		printf("Wrong waveform mode.\n");
		return -1;
	}

	memset(file_name , 0, sizeof(file_name));
	sprintf(file_name, "%s", argv[4]);
	fp_bmp = fopen(file_name, "rb");
	if (NULL == fp_bmp)
	{
		printf("can't open file: %s\n", file_name);
		return -1;
	}

	fd_fb = open(fb_dev, O_RDWR, 0);
	if (fd_fb < 0) 
	{
		perror("framebuffer device open failed");
		exit(-1);
	}
	
	ret = ioctl(fd_fb, FBIOGET_VSCREENINFO, &screen_info);
	if (ret < 0)
	{
		perror("ioctl failed");
		goto err0;
	}
	
	printf("Set the background to 16-bpp\n");
	screen_info.bits_per_pixel = 16;
	screen_info.grayscale = 0;
	screen_info.yoffset = 0;
	screen_info.rotate = FB_ROTATE_UR;
	screen_info.activate = FB_ACTIVATE_FORCE;
	screen_info.yres_virtual = screen_info.yres * 2;
	screen_info.xres_virtual = screen_info.xres;
	
	ret = ioctl(fd_fb, FBIOPUT_VSCREENINFO, &screen_info);
	if (ret < 0)
	{
		perror("ioctl failed");
		goto err0;
	}

	ret = ioctl(fd_fb, FBIOGET_VSCREENINFO, &screen_info);
	if (ret < 0)
	{
		perror("ioctl failed");
		goto err0;
	}
	
	//fb_size = screen_info.xres_virtual * screen_info.yres_virtual * screen_info.bits_per_pixel / 8;
	fb_size = screen_info.xres * screen_info.yres * screen_info.bits_per_pixel / 8;

	printf("screen_info.xres_virtual = %d\nscreen_info.yres_virtual = %d\nscreen_info.bits_per_pixel = %d\n",
		screen_info.xres_virtual, screen_info.yres_virtual, screen_info.bits_per_pixel);
	printf("screen_info.xres = %d\nscreen_info.yres = %d\nscreen_info.bits_per_pixel = %d\n",
		screen_info.xres, screen_info.yres, screen_info.bits_per_pixel);

	printf("Mem-Mapping FB0\n");

	/* Map the device to memory*/
	fbp = (unsigned short *)mmap(0, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if ((int)fbp <= 0)
	{
		printf("Error: failed to map framebuffer device 0 to memory.\n");
		ret = -1;
		goto err0;
	}
	
	//printf("Set to region update mode\n");
	auto_update_mode = AUTO_UPDATE_MODE_REGION_MODE;
	ret = ioctl(fd_fb, MXCFB_SET_AUTO_UPDATE_MODE, &auto_update_mode);
	if (ret < 0)
	{
		perror("ioctl failed");
		goto err1;
	}

	//printf("Set waveform modes\n");
	wv_modes.mode_init = 0;
	wv_modes.mode_du = 1;
	wv_modes.mode_gc4 = 3;
	wv_modes.mode_gc8 = 2;
	wv_modes.mode_gc16 = 2;
	wv_modes.mode_gc32 = 2;
	ret = ioctl(fd_fb, MXCFB_SET_WAVEFORM_MODES, &wv_modes);
	if (ret < 0)
	{
		perror("ioctl failed");
		goto err1;
	}

	printf("Set update scheme : Snapshot\n");
	//scheme = UPDATE_SCHEME_SNAPSHOT;
	scheme = UPDATE_SCHEME_QUEUE_AND_MERGE;
	ret = ioctl(fd_fb, MXCFB_SET_UPDATE_SCHEME, &scheme);
	if (ret < 0)
	{
		perror("ioctl failed");
		goto err1;
	}
	
	printf("1.Blank screen\n");
        memset(fbp, 0xFF, screen_info.xres_virtual * screen_info.yres * screen_info.bits_per_pixel / 8);
        update_to_display(0, 0, screen_info.xres, screen_info.yres, WAVEFORM_MODE_INIT, atoi(argv[1]), TRUE, atoi(argv[3]));
        sleep(1);
	
	printf("Display Picture %s\n", file_name);
	buf = (unsigned int *)malloc(screen_info.xres * screen_info.yres * screen_info.bits_per_pixel / 8);
	
	ret = convert_bmp24(fp_bmp, buf);
	if (0 != ret)
	{
		goto err1;
	}
	printf("************************************************\n");
	printf("************************************************\n");
	//copy_image_to_buffer(0, 0, screen_info.xres, screen_info.yres, buf, BUFFER_FB, &screen_info);
	memcpy(fbp, buf, screen_info.xres * screen_info.yres*2);
	update_to_display(0, 0, screen_info.xres, screen_info.yres, wf_mode, atoi(argv[1]), TRUE, atoi(argv[3]));

err1:
	munmap(fbp, fb_size);
err0:
	close(fd_fb);

	fclose(fp_bmp);
	return ret;
}
