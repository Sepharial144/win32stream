#include "WinRoutine.hpp"
#include "Exception/Exception.hpp"

HRESULT Win32Call(HRESULT status, const char* error)
{
    if (FAILED(status))
    {
        throw Win32Exception(error);
    }
    return status;
}

HANDLE Win32Handle(HANDLE handle, const char* error)
{
    if (handle != INVALID_HANDLE_VALUE)
    {
        throw InvalidHandleException(error);
    }
    return handle;
}