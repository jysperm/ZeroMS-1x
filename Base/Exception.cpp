#include "Exception.h"

namespace ZeroMS {
namespace Base {

/*!
    \class Exception
    \brief 异常基类

    是 \c std::exception 的 \c typdef
*/

/*!
    \class LogicException
    \brief 逻辑异常

    是 \c std::logic_error 的 \c typdef .
    用于未按照约定的顺序、方法使用函数/类，可以在编写时避免的错误.
*/

/*!
    \class ArgumentException
    \brief 参数异常

    模仿 \c std::runtime_error 的行为.
    用于传递给函数的参数不符合要求时，偏向于可在编写时避免.
*/

ArgumentException::ArgumentException(std::string msg):msg(msg)
{

}

ArgumentException::~ArgumentException() throw()
{

}

const char *ArgumentException::what() const throw()
{
    return this->msg.c_str();
}

/*!
    \class RuntimeException
    \brief 运行时异常

    是 \c std::runtime_error 的 \c typdef .
    用于不可预测的运行时错误.
*/

/*!
    \class RangeException
    \brief 范围异常

    用于参数的范围不符合要求时.
*/

/*!
    \class FormatException
    \brief 格式异常

    用于参数的格式不符合要求时.
*/

/*!
    \class NullException
    \brief 空指针异常

    用于发现空指针.
*/

/*!
    \class NetworkException
    \brief 网络异常
*/

/*!
    \class FileException
    \brief 文件异常
*/

/*!
    \class InputException
    \brief 用户输入异常
*/

}}  // namespace ZeroMS::Base
