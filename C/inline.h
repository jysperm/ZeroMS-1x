#ifndef INLINE_H
#define INLINE_H

//安全指针销毁函数(宏)
#define sDelete(p) if(p)delete p;p=0;

inline int P_checkVer(int n)
{
    //判断协议协议版本是否支持
    if(n==1)
	return 1;
    else
	return 0;
}

#endif // INLINE_H
