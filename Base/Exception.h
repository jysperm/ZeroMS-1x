#ifndef ZEROMS_BASE_EXCEPTION_H
#define ZEROMS_BASE_EXCEPTION_H

#include <stdexcept>

#ifndef Q_QDOC
namespace ZeroMS {
namespace Base {
#endif

typedef ::std::exception Exception;
typedef ::std::logic_error LogicException;
typedef ::std::runtime_error RuntimeException;

class ArgumentException : public Exception
{

};

class RangeException : public ArgumentException
{

};

class FormatException : public ArgumentException
{

};

class NullException : public ArgumentException
{

};

class NetworkException : public RuntimeException
{

};

class FileException : public RuntimeException
{

};

class InputException : public RuntimeException
{

};

#ifndef Q_QDOC
}}  // namespace ZeroMS::Base
#endif

#endif // ZEROMS_BASE_EXCEPTION_H
