/*
 * =====================================================================================
 *
 *       Filename:  gtksdl.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  一月 15, 2015
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

# ifndef __GTK_SDL_H__
# define __GTK_SDL_H__

#ifdef USE_GTKSDL

# include <stdlib.h>

# include <gtk/gtk.h>
# include <gdk/gdk.h>
# include <gtk/gtkwidget.h>
# include <gdk/gdkx.h>

# include <SDL/SDL.h>

# ifdef __cplusplus
extern "C" {
# endif

# define GTK_TYPE_SDL               (gtk_sdl_get_type())
# define GTK_SDL(obj)               (GTK_CHECK_CAST((obj), GTK_TYPE_SDL, GtkSDL))
# define GTK_SDL_CLASS(klass)       (GTK_CHECK_CLASS_CAST((klass), GTK_TYPE_SDL, GtkSDLClass))
# define GTK_IS_SDL(obj)            (GTK_CHECK_TYPE((obj), GTK_TYPE_SDL))
# define GTK_IS_SDL_CLASS(klass)    (GTK_CHECK_CLASS_TYPE((klass), GTK_TYPE_SDL))

typedef struct _GtkSDL		GtkSDL;
typedef struct _GtkSDLClass	GtkSDLClass;
typedef SDL_Rect            GtkSDLRect;

struct _GtkSDL {
    GtkWidget widget;

    SDL_Surface *surface;
    gint width, height, bpp;
    Uint16 flags;
};

struct _GtkSDLClass {
    GtkWidgetClass parent_class;
};

/**
 * @Synopsis SDL初始化
 *
 * @Returns 成功返回0，失败返回-1
 */
int gtk_sdl_init();

/**
 * @Synopsis SDL清除
 *
 * @Returns 成功返回0，失败返回-1
 */
int gtk_sdl_free();

/**
 * @Synopsis 注册gtk_sdl控件类型
 *
 * @Returns 
 */
GtkType gtk_sdl_get_type(void);

/**
 * @Synopsis 创建gtk_sdl对象，可传参数
 *
 * @Param width     SDL窗口宽度
 * @Param height    SDL窗口高度
 * @Param bpp       每像素的位深
 * @Param flags     创建SDL窗口的标识符
 *
 * @Returns 
 */
GtkWidget *gtk_sdl_new_params(gint width, gint height, gint bpp, Uint32 flags);

/**
 * @Synopsis        创建gtk_sdl对象，使用默认的位深和标识
 *
 * @Param width     SDL窗口宽度
 * @Param height    SDL窗口高度
 *
 * @Returns 
 */
GtkWidget *gtk_sdl_new(gint width, gint height);

/**
 * @Synopsis        调整SDL窗口大小
 *
 * @Param sdl
 * @Param width     调整后的窗口宽度
 * @Param height    调整后的窗口高度
 */
void gtk_sdl_scale(GtkSDL *sdl, gint width, gint height);

void gtk_sdl_update(GtkSDL *sdl, void *pixels, 
                    int width, int height, 
                    int depth, int pitch);

/**
 * @Synopsis 更新SDL显示区域
 *
 * @Param sdl
 * @Param pixels    图像数据
 * @Param width     图像宽度
 * @Param height    图像高度
 * @Param depth     色彩位深
 * @Param pitch     图像行字节大小
 */
void gtk_sdl_update_roi(GtkSDL *sdl, void *pixels, 
                        int width, int height, 
                        int depth, int pitch, 
                        GtkSDLRect *src, GtkSDLRect *des);

#ifdef __cplusplus
}
#endif

#endif

#endif /* __GTK_SDL_H__ */
