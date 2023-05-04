#ifndef _PROCESS_INTERNALS_HPP_
#define _PROCESS_INTERNALS_HPP_

#include <Windows.h>
#include <string>
#include <Tlhelp32.h>
#include <cstring>
#include <iostream>

namespace ProcessEngine {

	class ProcessInternals {
	public:
		explicit ProcessInternals();
		explicit ProcessInternals(const char* process_name);
		~ProcessInternals();
		void setProcessName(const char* process_name);
		DWORD findProcessId();
		HWND findHandleWindow();
		void findProcessName();
		HANDLE takeSnapshotProcess(DWORD processId);
		BYTE* findModuleBaseAddress();
		void processState() const;
		int32_t attach(DWORD access);
		RECT getWindowPosition();
		
		template<typename Value>
		Value readMemory(int32_t addr);

	private:
		const char* m_processName;
		std::string m_executionName;
		DWORD m_processId;
		BYTE* m_baseAddress;
		HANDLE m_processHandle;
		HWND m_handleWindow;

	};

} // namespace ProcessEngine

#endif // _PROCESS_INTERNALS_HPP_


