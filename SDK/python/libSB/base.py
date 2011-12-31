#coding=utf-8

import socket
from struct import pack,unpack
import select
from time import time as __time__
from thread import start_new_thread

time = lambda :int(__time__())

class BaseSB(object):
    '''零毫秒SB协议的收发实现
    '''

    def __init__(self,server='173.212.235.252',port=4321):
        self.server = server
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.connect((server,port))
        except socket.error, e:
            self.sock.close()
        self.startloop()
        self.recvfuncs = {}
        
        
    def receive(self,id):
        def install(func):
            self.recvfuncs[id] = func
            return func
        return install

    def send(self,type,data=None):
        irdy, ordy, erdy = select.select([], [self.sock], [], 5.0)
        assert ordy
        if data == None:
            packet = pack('!IIII',
                1,0,type,time()) #1代表版本号，0代表数据长度
        else:
            length = len(data)
            packet = pack('!IIII',
                1,length,type,time()) + data
        self.sock.send(packet)

    def startloop(self):
        self.loop = True
        start_new_thread(self._loop,())

    def _loop(self):
        while self.loop:
            data = self._recv()
            if data != None:
                try:
                    print data
                    func = self.recv[int(data[2])]
                    func(data)
                except:
                    pass

    def stoploop(self):
        self.loop = False

    def _recv(self, timeout = 0.1):
        irdy, ordy, erdy = select.select([self.sock], [], [], timeout)
        if not irdy:
            return None
        packet = self.sock.recv(1024)
        data = list(unpack('!IIII',packet[:16]))
        if data[1] > 0:
            data.append(''.join(unpack('!'+'s'*data[1],packet[16:])))
        return data
