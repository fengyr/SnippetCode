# encoding=utf8
import pygtk
pygtk.require('2.0')
import gtk
import gtk.glade
import os
import re
import lyric
from config import *
from mpplayer import *
# from sdlplayer import *

# 设置gtk.glade.XML对象，作为该模块的全局变量
def set_widget(w):
    global widget
    global Player

    widget = w
    Player = XPlayer()
    init_list(SEARCH_PATH)


def walk_dir(path):
    """ search dir and list file
            """
    for root, dirs, files in os.walk(path):
        for name in files:
            if os.path.splitext(name)[1] in MUSIC_TYPE:
                ALLFILE[os.path.splitext(name)[
                        0]] = str(os.path.join(root, name))

    return ALLFILE


def init_list(path):
    """ init list control
            """
    clist = widget['clist1']
    clist.clear()
    clist.set_column_width(0, 120)
    clist.set_column_width(1, 60)
    clist.set_column_width(2, 80)
    clist.set_column_title(0, "歌名")
    clist.set_column_title(1, "艺术家")
    clist.set_column_title(2, "专辑")
    clist.column_titles_show()

    walk_dir(path)
    read_mp3_info()
    for i in ALLFILE.keys():
        try:
            clist.append([i, TAGINFO[i]['artist'], TAGINFO[i]['album']])
        except KeyError, UnicodeDecodeError:
            continue


def read_mp3_info():
    tagMap = {'title': (3, 33),
              'artist': (33, 63),
              'album': (63, 93)}

    info = None
    for i in ALLFILE.keys():
        try:
            f = open(ALLFILE[i], "rb")
            try:
                f.seek(-128, 2)
                info = f.read(128)
                if info[0:3] == 'TAG':
                    TAGINFO[i] = {}
                    for tag, (start, end) in tagMap.items():
                        try:
                            TAGINFO[i][tag] = re.sub('\x00', '', info[
                                                     start:end]).decode('gbk')
                        except UnicodeDecodeError:
                            continue
                else:
                    TAGINFO[i] = {}
                    TAGINFO[i]['artist'] = os.path.splitext(ALLFILE[i])[1][1:]
                    TAGINFO[i]['album'] = '未知'
            finally:
                f.close()
        except:
            print "read file info err"
            continue

# 主窗口相关的类，该类负责处理UI的事件响应
class win:
    def on_window1_delete_event(obj, event):
        Player.clear()
        gtk.main_quit()

    def on_clist1_select_row(obj, row, column, event):
        clist = widget['clist1']
        # for i in range(row, widget['clist1'].rows):
        # name.append(ALLFILE[widget['clist1'].get_text(i, column)])
        name = ALLFILE[clist.get_text(row, 0)]
        title = Player.play(name)
        lyric.get_lyric(name)

        win = widget['window1']
        win.set_title(title)

    def on_stop_clicked(obj):
        title = Player.clear()

        win = widget['window1']
        win.set_title(title)

    def on_play_clicked(obj):
        title = Player.resume()

        win = widget['window1']
        win.set_title(title)

    def on_browser_clicked(obj):
        fs = widget['fileselection1']
        fs.show()

    def on_text_clicked(obj):
        global SHOWTEXT
        try:
            st = widget['songtext']
            if SHOWTEXT:
                st.hide()
            else:
                st.show()
            SHOWTEXT = not SHOWTEXT
        except:
            print 'lyric error'

# 文件选择窗口相关类，负责事件响应
class FileSelect:
    def on_ok_button1_clicked(obj):
        fs = widget['fileselection1']
        paths = fs.get_selections()
        for path in paths:
            if os.path.isdir(path):
                SEARCH_PATH = path
                init_list(SEARCH_PATH)
        fs.hide()

    def on_cancel_button1_clicked(obj):
        fs = widget['fileselection1']
        fs.hide()
