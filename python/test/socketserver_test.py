import SocketServer

class myserver( SocketServer.StreamRequestHandler ):
    def __init__(self, request, addr, server):
        print request, addr, server
        SocketServer.StreamRequestHandler.__init__(self, request, addr, server)

    def setup(self):
        print 'server setup'
        SocketServer.StreamRequestHandler.setup(self)

    def handle(self):
        print 'handle'
        print self.rfile.readline(1024)

    def finish(self):
        print 'server finish'
        SocketServer.StreamRequestHandler.finish(self)

if __name__ == '__main__':
    server = SocketServer.TCPServer( ('192.168.100.55', 9000), myserver )
    server.serve_forever()
