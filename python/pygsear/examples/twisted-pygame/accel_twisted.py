# pygsear
# Copyright (C) 2003 Lee Harr
#
#
# This file is part of pygsear.
#
# pygsear is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# pygsear is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with pygsear; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

from pygsear.Network import reactor

import pygsear
from pygsear import Game
from pygsear import Drawable
from pygsear import Path
from pygsear import conf



class TwistedGame(Game.TwistedGame):
    def initialize(self):
        self.set_background()

        CENTER = (conf.WINWIDTH/2, conf.WINHEIGHT/2)

        for c in range(10):
            dot = Drawable.Circle(color='random')
            p = Path.RandomAccelerationPathBounded(startLocation=CENTER)
            dot.set_path(p)
            self.sprites.add(dot)

        reactor.callLater(15, self.finish)

    def finish(self):
        reactor.stop()


game = TwistedGame(reactor=reactor, delay=0.2)
game.mainloop()

reactor.run()

