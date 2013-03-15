# encoding=utf8
import os
import sys

PLAYER = "/usr/bin/mplayer"

"""
使用mplayer解码音视频文件
"""
class XPlayer:
    def __init__(self):
        self.filename = ''
        pass

    def clear(self):
        os.system("killall %s > /dev/null 2>/dev/null" % PLAYER)
        print '停止当前播放 %s' % self.filename
        return '停止播放 %s' % os.path.basename(self.filename)

    def play(self, filename):
        """ use plugs play file
            """
        self.filename = filename
        self.clear()
        id = os.fork()
        if id == 0:
            cid = os.fork()
            if cid == 0:
                os.execv(PLAYER, ["", filename])
            else:
                sys.exit(0)
        else:
            os.wait()

        return os.path.basename(self.filename)

    def resume(self):
        if self.filename == '':
            return
        else:
            return self.play(self.filename)

    def __del__(self):
        pass
