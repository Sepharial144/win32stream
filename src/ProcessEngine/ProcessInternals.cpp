#include "ProcessEngine/ProcessInternals.hpp"
#include "WinRoutine.hpp"
#include "Exception/Exception.hpp"

namespace ProcessEngine {
	ProcessInternals::ProcessInternals()
		:m_processName{ nullptr },
		 m_executionName{},
		 m_processId{ 0 },
		 m_baseAddress{ 0x0 },
		 m_processHandle{ 0 },
		 m_handleWindow{ 0 }
	{
		m_executionName.reserve(MAX_PATH);
	}


	ProcessInternals::ProcessInternals(const char* process_name)
		:m_processName{ process_name },
		 m_executionName{},
		 m_processId{ 0 },
		 m_baseAddress{ 0x0 },
		 m_processHandle{ 0 },
		 m_handleWindow{ 0 }
	{
	}


	ProcessInternals::~ProcessInternals()
	{
	}


	void ProcessInternals::setProcessName(const char* process_name)
	{
		m_processName = process_name;
	}


	void ProcessInternals::processState() const
	{
		std::cout << "------------------------------------------------" << "\n"
			<< "Process Id: " << m_processId << "\n"
			<< "Process name: " << m_processName << "\n"
			<< "Execution name: " << m_executionName << "\n"
			<< "Handle Window: " << m_handleWindow << "\n"
			<< "Module base address: " << m_baseAddress << "\n"
			<< "Process Handle: " << m_processHandle << std::endl;
	}


	DWORD ProcessInternals::findProcessId()
	{
		m_handleWindow = findHandleWindow();
		if (m_handleWindow) {
			GetWindowThreadProcessId(m_handleWindow, &m_processId);
			return m_processId;
		}
		else 
		{
			throw("ProcessInternals: could not find process id");
		}
		return (DWORD)0;
	}


	HWND ProcessInternals::findHandleWindow()
	{
		return FindWindowA(0, (LPCSTR)m_processName);
	}


	void ProcessInternals::findProcessName()
	{
		DWORD processId = findProcessId();
		if (!processId)
		{
			throw Win32Exception("ProcessInternals: findProcessName process id is not valid");
			return;
		}

		HANDLE hSnapshot = takeSnapshotProcess(0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			throw Win32Exception("ProcessInternals: findProcessName snapwhot of process is invalid");
			return;
		}

		PROCESSENTRY32 currentProcessEntry32 = { 0 };
		currentProcessEntry32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &currentProcessEntry32))
		{
			m_executionName.clear();
			do {
				if (currentProcessEntry32.th32ProcessID == processId)
				{
					m_executionName.append(currentProcessEntry32.szExeFile);
					//std::wstring processName{ currentProcessEntry32.szExeFile };
					//std::copy(processName.begin(), processName.end(), std::back_inserter(m_executionName));
					std::cout << "ProcessInternals: findProcessName name of process has found ... " << m_executionName.c_str() << &std::endl;
					CloseHandle(hSnapshot);
					return;
				}
			} while (Process32Next(hSnapshot, &currentProcessEntry32));
		}
		throw Win32Exception("ProcessInternals: findProcessName snapshot does not contain process");
		CloseHandle(hSnapshot);
	}


	HANDLE ProcessInternals::takeSnapshotProcess(DWORD processId)
	{
		return Win32Handle(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId),
			"ProcessInternals: Could not take process snapshot");
	}


	BYTE* ProcessInternals::findModuleBaseAddress()
	{
		MODULEENTRY32 moduleEntry32 = { 0 };
		MODULEENTRY32 currentModuleEntry32 = { 0 };

		HANDLE hSnapshot = takeSnapshotProcess(m_processId);
			//CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _processId); // works only x64 application
			//CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32, _processId);
			//CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_processId);
		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			throw Win32Exception("ProcessInternals: findModuleBaseAddress snapwhot of process is invalid");
			return nullptr;
		}

		currentModuleEntry32.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &currentModuleEntry32))
		{
			do
			{
				if (!strcmp((char*)currentModuleEntry32.szModule, m_executionName.c_str()))
				{
				    moduleEntry32 = currentModuleEntry32;
					m_baseAddress = currentModuleEntry32.modBaseAddr;
					return m_baseAddress;
				}

			} while (Module32Next(hSnapshot, &currentModuleEntry32));
		}
		throw Win32Exception("ProcessInternals: findModuleBaseAddress could not find base address of module");
		CloseHandle(hSnapshot);
		return nullptr;
	};


	int32_t ProcessInternals::attach(DWORD access = PROCESS_VM_READ)
	{
		std::cout << access;
		if (m_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_processId)) {
			std::cout << "Process Handle: " << m_processHandle << &std::endl;
			return 1;
		}
		return 0;
	};


	RECT ProcessInternals::getWindowPosition()
	{
		RECT wPosition;
		HWND hWindow = findHandleWindow();
		GetWindowRect(hWindow, &wPosition);
		return wPosition;
	}


	template<typename Value>
	Value ProcessInternals::readMemory(int32_t addr)
	{
		Value value;
		if (m_processHandle)
		{
			ReadProcessMemory(m_processHandle, (LPVOID)addr, &value, sizeof(value), 0);
			return value;
		}
		else
		{
			throw Win32Exception("ProcessInternals: ReadMmeory could not read memory");
		}
		return (Value)(0);
	}
} // namespace ProcessEngine