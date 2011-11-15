#ifndef OGLOBAL_H
#define OGLOBAL_H


//
// 版本字符串
//
#define OMS_VERSION_STR     "0.1.0"
//
// 此VERSION为 (主版本号<<16)|(次版本号<<8)|(修正版本号)
//
#define OMS_VERSION         0x001000
//
// 可以这样使用： #if OMS_VERSION >= OMS_GET_VERSION(0,1,0)
//              // ...
//
#define OMS_GET_VERSION(major, minor, revision) ( (major<<16)|(minor<<8)|(revision) )

#define OMS_NAMESPACE_BEGIN namespace OMS \
{
#define OMS_NAMESPACE_END   }

OMS_NAMESPACE_BEGIN

/*class OCORESHARED_EXPORT Core;
class OCORESHARED_EXPORT Config;
class OCORESHARED_EXPORT Client;
class OCORESHARED_EXPORT Command;*/

OMS_NAMESPACE_END

#endif // OGLOBAL_H
