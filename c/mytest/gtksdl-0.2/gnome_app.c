#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <gtk/gtk.h>
#include <time.h>

#include "gtksdl.h"

static GtkBuilder *s_gtk_builder;
static GtkWidget *sdl_widget;
static int pic_id = 0;

static int width = 400;
static int height = 400;
static int cb_update(void *user_data)
{
    time_t now;
    struct tm *l_time;
    gchar buf[100];
    now=time((time_t *)NULL);
    l_time=localtime(&now);
    sprintf(buf,"%d:%d:%d",l_time->tm_hour,l_time->tm_min,l_time->tm_sec);
    printf("buf=%s\n", buf);

    GtkSDL *sdl = (GtkSDL*) user_data;
    char pic_path[10];
    memset(pic_path, 0, sizeof(pic_path));

    pic_id++;
    if (pic_id > 4) {
        pic_id = 1;
    }

    sprintf(pic_path, "./%d.bmp", pic_id);
    printf("image=%s\n", pic_path);
    cv::Mat img = cv::imread(pic_path);
    cvtColor(img, img, CV_BGR2RGB);

    /* cv::imshow("1", img); */

    gtk_sdl_update(sdl, img.data, img.cols, img.rows, 24, img.step);

    return 1;
}

gint AppInit(int argc, char *argv[])
{
    g_thread_init(NULL);
    gdk_threads_init();
	gtk_init(NULL, NULL);
    s_gtk_builder = gtk_builder_new();
    gtk_builder_add_from_file(s_gtk_builder, "./ui.glade", NULL);
    gtk_builder_connect_signals(s_gtk_builder, NULL);

    gtk_sdl_init();

	return(TRUE);
}

gint AppMainLoop(void )
{
    GtkWidget *window_top= (GtkWidget*)gtk_builder_get_object(s_gtk_builder, "window_top");
    GtkWidget *hbox1= (GtkWidget*)gtk_builder_get_object(s_gtk_builder, "hbox1");

	sdl_widget = gtk_sdl_new(400, 400);
    
    gint timeout_id = gtk_timeout_add(50, cb_update, (void*)sdl_widget);
    gtk_object_set_data( GTK_OBJECT (sdl_widget), "timeout_id", (gpointer)timeout_id );

    gtk_container_add(GTK_CONTAINER(hbox1), sdl_widget);
    gtk_widget_show(sdl_widget);

    gtk_widget_show(window_top);

    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();

	return(TRUE);
}

int main(int argc, char *argv[])
{

	/* Init the Aplication enviroment*/
	if ( !AppInit(argc, argv) ) {
		printf("\n[!] NOT initialised\n");
		return(-1);
	}

	/* Enter in the event handling main loop */
	if ( !AppMainLoop() )
	{
		printf("\n[!] Error entering app mail loop\n");
		return -1;
	}

    gtk_sdl_free();

	return 0;
}
