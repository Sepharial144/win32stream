#ifndef _WIN32STREAM_EXCEPTION_HPP_
#define _WIN32STREAM_EXCEPTION_HPP_
#include <string>
#include <exception>
#include <windows.h>


class Win32Exception : public std::exception
{
public:
    explicit Win32Exception()
        :m_winErrorCode(GetLastError())
    {
    }

    explicit Win32Exception(const char* error)
        :m_errorMessage(error) 
    {
        appendError();
    }

    virtual ~Win32Exception() noexcept {}

    virtual const char* what() const noexcept 
    {
        return m_errorMessage.c_str();
    }

protected:
    void appendError()
    {
        m_errorMessage.append(": ");
        m_errorMessage.append(std::to_string(m_winErrorCode));
    }

protected:
    DWORD m_winErrorCode;
    std::string m_errorMessage;
};

class DeviceException : public Win32Exception
{
public:
    explicit DeviceException() = delete;
    explicit DeviceException(const char* error)
    {
        m_errorMessage = error;
        appendError();
    }

    virtual ~DeviceException() noexcept {}

    virtual const char* what() const noexcept
    {
        return m_errorMessage.c_str();
    }
};

class InvalidHandleException : public Win32Exception
{
public:
    explicit InvalidHandleException() = delete;
    explicit InvalidHandleException(const char* error)
    {
        m_errorMessage = error;
        appendError();
    }

    virtual ~InvalidHandleException() noexcept {}

    virtual const char* what() const noexcept
    {
        return m_errorMessage.c_str();
    }
};

class InvalidHandleDeviceContextException : public Win32Exception
{
public:
    explicit InvalidHandleDeviceContextException() = delete;
    explicit InvalidHandleDeviceContextException(const char* error)
    {
        m_errorMessage = error;
        appendError();
    }

    virtual ~InvalidHandleDeviceContextException() noexcept {}

    virtual const char* what() const noexcept
    {
        return m_errorMessage.c_str();
    }
};


class InvalidHandleBitmapException : public Win32Exception
{
public:
    explicit InvalidHandleBitmapException() = delete;
    explicit InvalidHandleBitmapException(const char* error)
    {
        m_errorMessage = error;
        appendError();
    }

    virtual ~InvalidHandleBitmapException() noexcept {}

    virtual const char* what() const noexcept
    {
        return m_errorMessage.c_str();
    }
};


class InvalidHandleGDIObjectException : public Win32Exception
{
public:
    explicit InvalidHandleGDIObjectException() = delete;
    explicit InvalidHandleGDIObjectException(const char* error)
    {
        m_errorMessage = error;
        appendError();
    }

    virtual ~InvalidHandleGDIObjectException() noexcept {}

    virtual const char* what() const noexcept
    {
        return m_errorMessage.c_str();
    }
};


#endif // !_WIN32STREAM_EXCEPTION_HPP_