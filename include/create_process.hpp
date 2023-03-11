#if !defined(__4FF20D27_7FCE_4D4F_AD72_75CE1C09E531__)
#define __4FF20D27_7FCE_4D4F_AD72_75CE1C09E531__
#include "include.hpp"
#include "windows.hpp"
#include "reflecive.hpp"

namespace memade {

	static bool OnHookSandBoxCreateProcess(
		const PROCESS_INFORMATION& create_process_information) {
		bool result = false;
#if 0//!@ disabled
		do {
			std::string pe_buffer = Win::Read(R"(C:\google\chromium-dev\components\main.dll)");
			std::string shellcode;
			Reflecive::GenerateShellcode(pe_buffer, "api_object_init", "", EnShellCodeFlag::EN_SC_CLEARHEADER, shellcode);
			DWORD shellcode_size = static_cast<DWORD> (shellcode.size());
#if 0
			CONTEXT threadContext = { 0 };
			threadContext.ContextFlags = CONTEXT_ALL;
			if (FALSE == ::GetThreadContext(create_process_information.hThread, &threadContext))
				break;
			LPVOID lpBase = ::VirtualAllocEx(create_process_information.hProcess, nullptr, shellcode_size,
				MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (!lpBase)
				break;
			SIZE_T sizeNumberOfBytesWritten = 0;
			if (FALSE == ::WriteProcessMemory(create_process_information.hProcess, lpBase,
				shellcode.data(), shellcode_size, &sizeNumberOfBytesWritten))
				break;
#ifdef _WIN64
			threadContext.Rip = reinterpret_cast<decltype(threadContext.Rip)>(lpBase);
#else
			threadContext.Eip = reinterpret_cast<decltype(threadContext.Eip)>(lpBase);
#endif
			if (FALSE == ::SetThreadContext(create_process_information.hThread, &threadContext))
				break;
			::ResumeThread(create_process_information.hThread);
#endif
			auto m_lpBase = ::VirtualAllocEx(create_process_information.hProcess, nullptr, shellcode_size,
				MEM_COMMIT | MEM_RESERVE,
				PAGE_EXECUTE_READWRITE);
			if (!m_lpBase)
				break;
			SIZE_T sizeNumberOfBytesWritten = 0;
			if (0 == ::WriteProcessMemory(create_process_information.hProcess, m_lpBase,
				shellcode.c_str(), shellcode_size, &sizeNumberOfBytesWritten))
				break;
			HANDLE hRemoteThread = ::CreateRemoteThread(create_process_information.hProcess, NULL, 1024 * 1024, (LPTHREAD_START_ROUTINE)m_lpBase, nullptr, (DWORD)NULL, NULL);
			if (!hRemoteThread)
				break;
			::WaitForSingleObject(hRemoteThread, INFINITE);
			DWORD dwThreadExitCode = 0;
			::GetExitCodeThread(hRemoteThread, &dwThreadExitCode);
			::CloseHandle(hRemoteThread);

			result = true;
		} while (0);
#endif///disabled
		return result;
	}

}///namespace memade

/// /*_ Memade®（新生™） _**/
/// /*_ Thu, 09 Mar 2023 13:45:34 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__4FF20D27_7FCE_4D4F_AD72_75CE1C09E531__