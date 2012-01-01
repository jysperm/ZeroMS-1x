#coding=utf-8

import socket
from struct import pack,unpack
import select
from time import time as _time
import os
    
def toEcho(str):
    return str.decode('utf-8').encode((os.name == 'posix' and 'utf-8' or 'cp936'))

time = lambda :int(_time())

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
        self.loop = True
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

    def mainloop(self):
        while self.loop:
            data = self._recv()
            if data != None:
                try:
                    func = self.recvfuncs[data[2]]
                    func(data)
                except:
                    pass

    def stoploop(self):
        self.loop = False

    def _recv(self, timeout = 0.1):
        irdy, ordy, erdy = select.select([self.sock], [], [], timeout)
        try:
            packet = self.sock.recv(16)
            data = list(unpack('!IIII',packet))
            if data[1] > 0:
                data.append(toEcho(self.sock.recv(data[1])))
            return data
        except:
            return None
