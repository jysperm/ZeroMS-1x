#ifndef PUBLIC_CONST_H
#define PUBLIC_CONST_H

//全局通用常量定义文件
//ps.使用const常量会经过C++的类型检查！尽量减少使用预处理宏

//尽量减少包含头文件，尽可能使用前置声明
#include <QString>

//该宏几乎每个文件都要用到，考虑性能问题，不放在inline.h里
//注意，如果要操作的指针是只读的，不要使用这个宏，直接用delete关键字就行
#define DELETE(p) {if(p)delete (p);(p)=0;};

//服务器端口
const int SERVER_PORT = 4321;

//主群群号
const QString MAIN_GROUP = "*0";

#endif // PUBLIC_CONST_H
