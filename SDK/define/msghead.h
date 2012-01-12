#ifndef OMS_SDK_DEFINE_MSGHEAD_H
#define OMS_SDK_DEFINE_MSGHEAD_H

//  http://0-ms.org/wiki/protocol/p2
//零毫秒SB2.x数据包头部示例
//该文件仅作示例，请勿在实际开发中使用

class MsgHead
{
public:
    int pVer;
    unsigned int dataLength;
    int MsgType;
    unsigned int sendTime;
};

#endif // OMS_SDK_DEFINE_MSGHEAD_H
