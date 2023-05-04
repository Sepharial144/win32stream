#ifndef _WIN32STREAM_CAPTURE_CHECKER_HPP_
#define _WIN32STREAM_CAPTURE_CHECKER_HPP_

#include "CommonTypes.hpp"
#include "Exception.hpp"

#include <type_traits>

class Win32CaptureChecker
{
public:
	explicit Win32CaptureChecker() {};
	virtual ~Win32CaptureChecker() {};

protected:
	template <typename Win32ValueType>
	void checkWin32Value(const Win32ValueType win32Value);

	template <typename Win32ValueType>
	void checkWin32ValueMessage(const Win32ValueType win32Value, const char* error);

	HRESULT Win32Call(HRESULT status, const char* error)
	{
		if (FAILED(status))
			throw Win32Exception(error);
		return status;
	}
};


template <typename Win32ValueType>
void Win32CaptureChecker::checkWin32Value(const Win32ValueType win32Value)
{
	if constexpr (std::is_same<HWND, Win32ValueType>::value)
	{
		checkWin32ValueMessage<Win32ValueType>(win32Value, "Handle of deivce context is null");
	} 
	else if constexpr (std::is_same<HDC, Win32ValueType>::value)
	{
		checkWin32ValueMessage<Win32ValueType>(win32Value, "Handle device context is empty");
	}
	else if constexpr (std::is_same<HBITMAP, Win32ValueType>::value)
	{
		checkWin32ValueMessage<Win32ValueType>(win32Value, "Handle bitmap is empty");
	}
	else if constexpr (std::is_same<HGDIOBJ, Win32ValueType>::value)
	{
		checkWin32ValueMessage<Win32ValueType>(win32Value, "Handle of graphic interface object is empty");
	}
}


template <typename Win32ValueType>
void Win32CaptureChecker::checkWin32ValueMessage(const Win32ValueType win32Value, const char* error)
{
	if (!win32Value)
	{
		if constexpr (std::is_same<HWND, Win32ValueType>::value)
		{
			throw InvalidHandleException(error);
		}
		else if constexpr (std::is_same<HDC, Win32ValueType>::value)
		{
			throw InvalidHandleDeviceContextException(error);
		}
		else if constexpr (std::is_same<HBITMAP, Win32ValueType>::value)
		{
			throw InvalidHandleBitmapException(error);
		}
		else if constexpr (std::is_same<HGDIOBJ, Win32ValueType>::value)
		{
			throw InvalidHandleGDIObjectException(error);
		}
	}
}

#endif // _WIN32STREAM_CAPTURE_CHECKER_HPP_
