#ifndef _WIN_ROUTINE_HPP_
#define _WIN_ROUTINE_HPP_

#include <windows.h>

HRESULT Win32Call(HRESULT status, const char* error);
HANDLE Win32Handle(HANDLE handle, const char* error);

#endif // _WIN_ROUTINE_HPP_
