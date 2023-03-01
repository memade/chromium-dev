﻿// Copyright 2013 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/chrome_elf/chrome_elf_main.h"

#include <assert.h>
#include <windows.h>

#include "chrome/chrome_elf/chrome_elf_security.h"
#include "chrome/chrome_elf/crash/crash_helper.h"
#include "chrome/chrome_elf/third_party_dlls/beacon.h"
#include "chrome/chrome_elf/third_party_dlls/main.h"
#include "chrome/install_static/install_details.h"
#include "chrome/install_static/install_util.h"
#include "chrome/install_static/product_install_details.h"
#include "chrome/install_static/user_data_dir.h"

#include "base/process/process_handle.h"
#include "../../../include/chromium.hpp"
// This function is exported from the DLL so that it can be called by WinMain
// after startup has completed in the browser process. For non-browser processes
// it will be called inside the DLL loader lock so it should do as little as
// possible to prevent deadlocks.
void SignalInitializeCrashReporting() {
	if (!elf_crash::InitializeCrashReporting()) {
#ifdef _DEBUG
		assert(false);
#endif  // _DEBUG
	}
}

void SignalChromeElf() {
	third_party_dlls::ResetBeacon();
}

bool GetUserDataDirectoryThunk(wchar_t* user_data_dir,
	size_t user_data_dir_length,
	wchar_t* invalid_user_data_dir,
	size_t invalid_user_data_dir_length) {
	std::wstring user_data_dir_str, invalid_user_data_dir_str;
	bool ret = install_static::GetUserDataDirectory(&user_data_dir_str,
		&invalid_user_data_dir_str);
	assert(ret);
	install_static::IgnoreUnused(ret);
	wcsncpy_s(user_data_dir, user_data_dir_length, user_data_dir_str.c_str(),
		_TRUNCATE);
	wcsncpy_s(invalid_user_data_dir, invalid_user_data_dir_length,
		invalid_user_data_dir_str.c_str(), _TRUNCATE);

	return true;
}



#if MEMADE_BROWSER_PROCESS_CHEILD_HOOK
#include <stdio.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <io.h>
#include <mutex>
#include <map>
#include <set>
#include <functional>
#include "D:\\github\\Windows\\3rdparty\\rapidjson\\include\\rapidjson\\reader.h"
#include "D:\\github\\Windows\\3rdparty\\rapidjson\\include\\rapidjson\\document.h"
#include "D:\\github\\Windows\\3rdparty\\rapidjson\\include\\rapidjson\\rapidjson.h"
#include "D:\\github\\Windows\\3rdparty\\rapidjson\\include\\rapidjson\\stringbuffer.h"
#include "D:\\github\\Windows\\3rdparty\\rapidjson\\include\\rapidjson\\writer.h"
#include "D:\\github\\Windows\\3rdparty\\rapidjson\\include\\rapidjson\\stream.h"
namespace memade {
	using tfEnumFolderNode = std::map<std::string, std::string>;

	class Win final {
	public:
		inline static std::string GetModulePathA(const HINSTANCE& hModule = nullptr);
		inline static std::wstring GetModulePathW(const HINSTANCE& hModule = nullptr);
		inline static std::string GetModulePathnameA(const HINSTANCE& hModule = nullptr);
		inline static std::wstring GetModulePathnameW(const HINSTANCE& hModule = nullptr);
		inline static std::string EnumCorrectPath(const std::string& in, const char& replace_at = '\\', const bool& isFolder = true);
		inline static void EnumFoldersAndFiles(
			const std::string& Path,
			tfEnumFolderNode& Folders,
			tfEnumFolderNode& Files,
			const char* FileFilter = "*.*",
			bool bSleepDirect = false,
			const std::function<void(const std::string&, const std::string&, const _finddata_t&)>& enumcb = nullptr);
	public:
		class Json final {
		public:
			inline static std::string to_string(const rapidjson::Value&);
		};
		class File final {
		public:
			inline static std::string Read(const std::string& FilePathname, const std::string& OpenMode = "rb", const int& _ShFlag = _SH_DENYNO);
			inline static void ParseA(const std::string& inStrContend, const int& nDelimiter, std::vector<std::string>& Output);
		};
		class PE final {
		public:
			using HMEMORYMODULE = void*;
			using HMEMORYRSRC = void*;
			using HCUSTOMMODULE = void*;
			using CustomAllocFunc = LPVOID(*)(LPVOID, SIZE_T, DWORD, DWORD, void*);
			using CustomFreeFunc = BOOL(*)(LPVOID, SIZE_T, DWORD, void*);
			using CustomLoadLibraryFunc = HCUSTOMMODULE(LPCSTR, void*);
			using CustomGetProcAddressFunc = FARPROC(*)(HCUSTOMMODULE, LPCSTR, void*);
			using CustomFreeLibraryFunc = void (*)(HCUSTOMMODULE, void*);
		public:
			static HMEMORYMODULE MemoryLoadLibrary(const void*, size_t);
			static HMEMORYMODULE MemoryLoadLibraryEx(const void*, size_t,
				CustomAllocFunc,
				CustomFreeFunc,
				CustomLoadLibraryFunc,
				CustomGetProcAddressFunc,
				CustomFreeLibraryFunc,
				void*);
			static FARPROC MemoryGetProcAddress(HMEMORYMODULE, LPCSTR);
			static void MemoryFreeLibrary(HMEMORYMODULE);
			static	int MemoryCallEntryPoint(HMEMORYMODULE);
			static HMEMORYRSRC MemoryFindResource(HMEMORYMODULE, LPCTSTR, LPCTSTR);
			static HMEMORYRSRC MemoryFindResourceEx(HMEMORYMODULE, LPCTSTR, LPCTSTR, WORD);
			static DWORD MemorySizeofResource(HMEMORYMODULE, HMEMORYRSRC);
			static LPVOID MemoryLoadResource(HMEMORYMODULE, HMEMORYRSRC);
			static int MemoryLoadString(HMEMORYMODULE, UINT, LPTSTR, int);
			static int MemoryLoadStringEx(HMEMORYMODULE, UINT, LPTSTR, int, WORD);
			static LPVOID MemoryDefaultAlloc(LPVOID, SIZE_T, DWORD, DWORD, void*);
			static BOOL MemoryDefaultFree(LPVOID, SIZE_T, DWORD, void*);
			static HCUSTOMMODULE MemoryDefaultLoadLibrary(const char*, void*);
			static FARPROC MemoryDefaultGetProcAddress(HCUSTOMMODULE, const char*, void*);
			static void MemoryDefaultFreeLibrary(HCUSTOMMODULE, void*);
		};
	};

	class PluginModule final {
	public:
		inline PluginModule(const std::string& plugin_pepathname);
		inline ~PluginModule();
		inline void Release() const;
	public:
		inline void Init();
		inline void UnInit();
		inline void* Identify() const;
		inline bool Ready() const;
		inline tf_memade_plugin_common_function Search(const std::string&) const;
	private:
		bool m_ready = false;
		void* m_module_handle = nullptr;
		tf_memade_plugin_init m_module_init = nullptr;
		tf_memade_plugin_uninit m_module_uninit = nullptr;
		const std::string m_plugin_pepathname;
		std::map<std::string, tf_memade_plugin_common_function> m_plugin_function_s;
	};

	class Chromium final {
		std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
	public:
		inline Chromium();
		inline ~Chromium();
	public:
		inline void Init();
		inline void UnInit();
		inline const DWORD& CurrentParentProcessId() const;
	public:
		inline tf_memade_plugin_common_function FindPluginFunction(const std::string&) const;
	private:
		DWORD m_CurrentParentProcessId = 0;
		std::map<void*, PluginModule*> m_plugin_module_s;
	};


	static memade::Chromium* __gspChromium = nullptr;
}///namespace memade
#endif///MEMADE_BROWSER_PROCESS_CHEILD_HOOK




// DllMain
// -------
// Warning: The OS loader lock is held during DllMain.  Be careful.
// https://msdn.microsoft.com/en-us/library/windows/desktop/dn633971.aspx
//
// - Note: Do not use install_static::GetUserDataDir from inside DllMain.
//         This can result in path expansion that triggers secondary DLL loads,
//         that will blow up with the loader lock held.
//         https://bugs.chromium.org/p/chromium/issues/detail?id=748949#c18
BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		install_static::InitializeProductDetailsForPrimaryModule();
		install_static::InitializeProcessType();

#if MEMADE_MAIN_SWITCH && MEMADE_BROWSER_PROCESS_CHEILD_HOOK
		memade::__gspChromium = new memade::Chromium();
		memade::__gspChromium->Init();
#endif///#if MEMADE_MAIN_SWITCH && MEMADE_BROWSER_PROCESS_CHEILD_HOOK

		if (install_static::IsBrowserProcess()) {
			__try {
				// Disable third party extension points.
				elf_security::EarlyBrowserSecurity();

				// Initialize the blocking of third-party DLLs if the initialization of
				// the safety beacon succeeds.
				if (third_party_dlls::LeaveSetupBeacon())
					third_party_dlls::Init();
			}
			__except (elf_crash::GenerateCrashDump(GetExceptionInformation())) {
			}
		}
		else if (!install_static::IsCrashpadHandlerProcess()) {
			SignalInitializeCrashReporting();
			// CRT on initialization installs an exception filter which calls
			// TerminateProcess. We need to hook CRT's attempt to set an exception.
			elf_crash::DisableSetUnhandledExceptionFilter();
		}
	}
	else if (reason == DLL_PROCESS_DETACH) {
#if MEMADE_MAIN_SWITCH && MEMADE_BROWSER_PROCESS_CHEILD_HOOK
		if (memade::__gspChromium) {
			memade::__gspChromium->UnInit();
			delete memade::__gspChromium;
			memade::__gspChromium = nullptr;
		}
#endif///#if MEMADE_MAIN_SWITCH && MEMADE_BROWSER_PROCESS_CHEILD_HOOK

		elf_crash::ShutdownCrashReporting();
	}
	return TRUE;
}

void DumpProcessWithoutCrash() {
	elf_crash::DumpWithoutCrashing();
}

void SetMetricsClientId(const char* client_id) {
	elf_crash::SetMetricsClientIdImpl(client_id);
}

bool IsBrowserProcess() {
	return install_static::IsBrowserProcess();
}

bool IsExtensionPointDisableSet() {
	return elf_security::IsExtensionPointDisableSet();
}


















#if MEMADE_BROWSER_PROCESS_CHEILD_HOOK
namespace memade {
#if _MSC_VER
	// Disable warning about data -> function pointer conversion
#pragma warning(disable: 4055)
	// C4244: conversion from 'uintptr_t' to 'DWORD', possible loss of data.
#pragma warning(error: 4244)
// C4267: conversion from 'size_t' to 'int', possible loss of data.
#pragma warning(error: 4267)
//#define inline __inline
#endif
#ifndef IMAGE_SIZEOF_BASE_RELOCATION
// Vista SDKs no longer define IMAGE_SIZEOF_BASE_RELOCATION!?
#define IMAGE_SIZEOF_BASE_RELOCATION (sizeof(IMAGE_BASE_RELOCATION))
#endif
#if !defined(ARCH_CPU_32_BITS)
#define _WIN64 1
#endif
#ifdef _WIN64
#define HOST_MACHINE IMAGE_FILE_MACHINE_AMD64
#else
#define HOST_MACHINE IMAGE_FILE_MACHINE_I386
#endif

	struct ExportNameEntry {
		LPCSTR name;
		WORD idx;
	};

	typedef BOOL(WINAPI* DllEntryProc)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);
	typedef int (WINAPI* ExeEntryProc)(void);

#ifdef _WIN64
	typedef struct POINTER_LIST {
		struct POINTER_LIST* next;
		void* address;
	} POINTER_LIST;
#endif

	typedef struct tagMEMORYMODULE {
		PIMAGE_NT_HEADERS headers;
		unsigned char* codeBase;
		Win::PE::HCUSTOMMODULE* modules;
		int numModules;
		BOOL initialized;
		BOOL isDLL;
		BOOL isRelocated;
		Win::PE::CustomAllocFunc alloc;
		Win::PE::CustomFreeFunc free;
		Win::PE::CustomLoadLibraryFunc* loadLibrary;
		Win::PE::CustomGetProcAddressFunc getProcAddress;
		Win::PE::CustomFreeLibraryFunc freeLibrary;
		struct ExportNameEntry* nameExportsTable;
		void* userdata;
		ExeEntryProc exeEntry;
		DWORD pageSize;
#ifdef _WIN64
		POINTER_LIST* blockedMemory;
#endif
	} MEMORYMODULE, * PMEMORYMODULE;

	typedef struct {
		LPVOID address;
		LPVOID alignedAddress;
		SIZE_T size;
		DWORD characteristics;
		BOOL last;
	} SECTIONFINALIZEDATA, * PSECTIONFINALIZEDATA;

#define GET_HEADER_DICTIONARY(module, idx)  &(module)->headers->OptionalHeader.DataDirectory[idx]

	static inline uintptr_t
		AlignValueDown(uintptr_t value, uintptr_t alignment) {
		return value & ~(alignment - 1);
	}

	static inline LPVOID
		AlignAddressDown(LPVOID address, uintptr_t alignment) {
		return (LPVOID)AlignValueDown((uintptr_t)address, alignment);
	}

	static inline size_t
		AlignValueUp(size_t value, size_t alignment) {
		return (value + alignment - 1) & ~(alignment - 1);
	}

	static inline void*
		OffsetPointer(void* data, ptrdiff_t offset) {
		return (void*)((uintptr_t)data + offset);
	}

	static inline void
		OutputLastError(const char* msg)
	{
#ifndef DEBUG_OUTPUT
		UNREFERENCED_PARAMETER(msg);
#else
		LPVOID tmp;
		char* tmpmsg;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&tmp, 0, NULL);
		tmpmsg = (char*)LocalAlloc(LPTR, strlen(msg) + strlen(tmp) + 3);
		sprintf(tmpmsg, "%s: %s", msg, tmp);
		OutputDebugString(tmpmsg);
		LocalFree(tmpmsg);
		LocalFree(tmp);
#endif
	}

#ifdef _WIN64
	static void
		FreePointerList(POINTER_LIST* head, Win::PE::CustomFreeFunc freeMemory, void* userdata)
	{
		POINTER_LIST* node = head;
		while (node) {
			POINTER_LIST* next;
			freeMemory(node->address, 0, MEM_RELEASE, userdata);
			next = node->next;
			free(node);
			node = next;
		}
	}
#endif

	static BOOL
		CheckSize(size_t size, size_t expected) {
		if (size < expected) {
			SetLastError(ERROR_INVALID_DATA);
			return FALSE;
		}

		return TRUE;
	}

	static BOOL
		CopySections(const unsigned char* data, size_t size, PIMAGE_NT_HEADERS old_headers, PMEMORYMODULE module)
	{
		int i, section_size;
		unsigned char* codeBase = module->codeBase;
		unsigned char* dest;
		PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(module->headers);
		for (i = 0; i < module->headers->FileHeader.NumberOfSections; i++, section++) {
			if (section->SizeOfRawData == 0) {
				// section doesn't contain data in the dll itself, but may define
				// uninitialized data
				section_size = old_headers->OptionalHeader.SectionAlignment;
				if (section_size > 0) {
					dest = (unsigned char*)module->alloc(codeBase + section->VirtualAddress,
						section_size,
						MEM_COMMIT,
						PAGE_READWRITE,
						module->userdata);
					if (dest == NULL) {
						return FALSE;
					}

					// Always use position from file to support alignments smaller
					// than page size (allocation above will align to page size).
					dest = codeBase + section->VirtualAddress;
					// NOTE: On 64bit systems we truncate to 32bit here but expand
					// again later when "PhysicalAddress" is used.
					section->Misc.PhysicalAddress = (DWORD)((uintptr_t)dest & 0xffffffff);
					memset(dest, 0, section_size);
				}

				// section is empty
				continue;
			}

			if (!CheckSize(size, section->PointerToRawData + section->SizeOfRawData)) {
				return FALSE;
			}

			// commit memory block and copy data from dll
			dest = (unsigned char*)module->alloc(codeBase + section->VirtualAddress,
				section->SizeOfRawData,
				MEM_COMMIT,
				PAGE_READWRITE,
				module->userdata);
			if (dest == NULL) {
				return FALSE;
			}

			// Always use position from file to support alignments smaller
			// than page size (allocation above will align to page size).
			dest = codeBase + section->VirtualAddress;
			memcpy(dest, data + section->PointerToRawData, section->SizeOfRawData);
			// NOTE: On 64bit systems we truncate to 32bit here but expand
			// again later when "PhysicalAddress" is used.
			section->Misc.PhysicalAddress = (DWORD)((uintptr_t)dest & 0xffffffff);
		}

		return TRUE;
	}

	// Protection flags for memory pages (Executable, Readable, Writeable)
	static int ProtectionFlags[2][2][2] = {
					{
			// not executable
			{PAGE_NOACCESS, PAGE_WRITECOPY},
			{PAGE_READONLY, PAGE_READWRITE},
	}, {
		// executable
		{PAGE_EXECUTE, PAGE_EXECUTE_WRITECOPY},
		{PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE},
	},
	};

	static SIZE_T
		GetRealSectionSize(PMEMORYMODULE module, PIMAGE_SECTION_HEADER section) {
		DWORD size = section->SizeOfRawData;
		if (size == 0) {
			if (section->Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA) {
				size = module->headers->OptionalHeader.SizeOfInitializedData;
			}
			else if (section->Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA) {
				size = module->headers->OptionalHeader.SizeOfUninitializedData;
			}
		}
		return (SIZE_T)size;
	}

	static BOOL
		FinalizeSection(PMEMORYMODULE module, PSECTIONFINALIZEDATA sectionData) {
		DWORD protect, oldProtect;
		BOOL executable;
		BOOL readable;
		BOOL writeable;

		if (sectionData->size == 0) {
			return TRUE;
		}

		if (sectionData->characteristics & IMAGE_SCN_MEM_DISCARDABLE) {
			// section is not needed any more and can safely be freed
			if (sectionData->address == sectionData->alignedAddress &&
				(sectionData->last ||
					module->headers->OptionalHeader.SectionAlignment == module->pageSize ||
					(sectionData->size % module->pageSize) == 0)
				) {
				// Only allowed to decommit whole pages
				module->free(sectionData->address, sectionData->size, MEM_DECOMMIT, module->userdata);
			}
			return TRUE;
		}

		// determine protection flags based on characteristics
		executable = (sectionData->characteristics & IMAGE_SCN_MEM_EXECUTE) != 0;
		readable = (sectionData->characteristics & IMAGE_SCN_MEM_READ) != 0;
		writeable = (sectionData->characteristics & IMAGE_SCN_MEM_WRITE) != 0;
		protect = ProtectionFlags[executable][readable][writeable];
		if (sectionData->characteristics & IMAGE_SCN_MEM_NOT_CACHED) {
			protect |= PAGE_NOCACHE;
		}

		// change memory access flags
		if (VirtualProtect(sectionData->address, sectionData->size, protect, &oldProtect) == 0) {
			OutputLastError("Error protecting memory page");
			return FALSE;
		}

		return TRUE;
	}

	static BOOL
		FinalizeSections(PMEMORYMODULE module)
	{
		int i;
		PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(module->headers);
#ifdef _WIN64
		// "PhysicalAddress" might have been truncated to 32bit above, expand to
		// 64bits again.
		uintptr_t imageOffset = ((uintptr_t)module->headers->OptionalHeader.ImageBase & 0xffffffff00000000);
#else
		static const uintptr_t imageOffset = 0;
#endif
		SECTIONFINALIZEDATA sectionData;
		sectionData.address = (LPVOID)((uintptr_t)section->Misc.PhysicalAddress | imageOffset);
		sectionData.alignedAddress = AlignAddressDown(sectionData.address, module->pageSize);
		sectionData.size = GetRealSectionSize(module, section);
		sectionData.characteristics = section->Characteristics;
		sectionData.last = FALSE;
		section++;

		// loop through all sections and change access flags
		for (i = 1; i < module->headers->FileHeader.NumberOfSections; i++, section++) {
			LPVOID sectionAddress = (LPVOID)((uintptr_t)section->Misc.PhysicalAddress | imageOffset);
			LPVOID alignedAddress = AlignAddressDown(sectionAddress, module->pageSize);
			SIZE_T sectionSize = GetRealSectionSize(module, section);
			// Combine access flags of all sections that share a page
			// TODO(fancycode): We currently share flags of a trailing large section
			//   with the page of a first small section. This should be optimized.
			if (sectionData.alignedAddress == alignedAddress || (uintptr_t)sectionData.address + sectionData.size > (uintptr_t) alignedAddress) {
				// Section shares page with previous
				if ((section->Characteristics & IMAGE_SCN_MEM_DISCARDABLE) == 0 || (sectionData.characteristics & IMAGE_SCN_MEM_DISCARDABLE) == 0) {
					sectionData.characteristics = (sectionData.characteristics | section->Characteristics) & ~IMAGE_SCN_MEM_DISCARDABLE;
				}
				else {
					sectionData.characteristics |= section->Characteristics;
				}
				sectionData.size = (((uintptr_t)sectionAddress) + ((uintptr_t)sectionSize)) - (uintptr_t)sectionData.address;
				continue;
			}

			if (!FinalizeSection(module, &sectionData)) {
				return FALSE;
			}
			sectionData.address = sectionAddress;
			sectionData.alignedAddress = alignedAddress;
			sectionData.size = sectionSize;
			sectionData.characteristics = section->Characteristics;
		}
		sectionData.last = TRUE;
		if (!FinalizeSection(module, &sectionData)) {
			return FALSE;
		}
		return TRUE;
	}

	static BOOL
		ExecuteTLS(PMEMORYMODULE module)
	{
		unsigned char* codeBase = module->codeBase;
		PIMAGE_TLS_DIRECTORY tls;
		PIMAGE_TLS_CALLBACK* callback;

		PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY(module, IMAGE_DIRECTORY_ENTRY_TLS);
		if (directory->VirtualAddress == 0) {
			return TRUE;
		}

		tls = (PIMAGE_TLS_DIRECTORY)(codeBase + directory->VirtualAddress);
		callback = (PIMAGE_TLS_CALLBACK*)tls->AddressOfCallBacks;
		if (callback) {
			while (*callback) {
				(*callback)((LPVOID)codeBase, DLL_PROCESS_ATTACH, NULL);
				callback++;
			}
		}
		return TRUE;
	}

	static BOOL
		PerformBaseRelocation(PMEMORYMODULE module, ptrdiff_t delta)
	{
		unsigned char* codeBase = module->codeBase;
		PIMAGE_BASE_RELOCATION relocation;

		PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY(module, IMAGE_DIRECTORY_ENTRY_BASERELOC);
		if (directory->Size == 0) {
			return (delta == 0);
		}

		relocation = (PIMAGE_BASE_RELOCATION)(codeBase + directory->VirtualAddress);
		for (; relocation->VirtualAddress > 0; ) {
			DWORD i;
			unsigned char* dest = codeBase + relocation->VirtualAddress;
			unsigned short* relInfo = (unsigned short*)OffsetPointer(relocation, IMAGE_SIZEOF_BASE_RELOCATION);
			for (i = 0; i < ((relocation->SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / 2); i++, relInfo++) {
				// the upper 4 bits define the type of relocation
				int type = *relInfo >> 12;
				// the lower 12 bits define the offset
				int offset = *relInfo & 0xfff;

				switch (type)
				{
				case IMAGE_REL_BASED_ABSOLUTE:
					// skip relocation
					break;

				case IMAGE_REL_BASED_HIGHLOW:
					// change complete 32 bit address
				{
					DWORD* patchAddrHL = (DWORD*)(dest + offset);
					*patchAddrHL += (DWORD)delta;
				}
				break;

#ifdef _WIN64
				case IMAGE_REL_BASED_DIR64:
				{
					ULONGLONG* patchAddr64 = (ULONGLONG*)(dest + offset);
					*patchAddr64 += (ULONGLONG)delta;
				}
				break;
#endif

				default:
					//printf("Unknown relocation: %d\n", type);
					break;
				}
			}

			// advance to next relocation block
			relocation = (PIMAGE_BASE_RELOCATION)OffsetPointer(relocation, relocation->SizeOfBlock);
		}
		return TRUE;
	}

	static BOOL
		BuildImportTable(PMEMORYMODULE module)
	{
		unsigned char* codeBase = module->codeBase;
		PIMAGE_IMPORT_DESCRIPTOR importDesc;
		BOOL result = TRUE;

		PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY(module, IMAGE_DIRECTORY_ENTRY_IMPORT);
		if (directory->Size == 0) {
			return TRUE;
		}

		importDesc = (PIMAGE_IMPORT_DESCRIPTOR)(codeBase + directory->VirtualAddress);
		for (; !IsBadReadPtr(importDesc, sizeof(IMAGE_IMPORT_DESCRIPTOR)) && importDesc->Name; importDesc++) {
			uintptr_t* thunkRef;
			FARPROC* funcRef;
			Win::PE::HCUSTOMMODULE* tmp;
			Win::PE::HCUSTOMMODULE handle = module->loadLibrary((LPCSTR)(codeBase + importDesc->Name), module->userdata);
			if (handle == NULL) {
				SetLastError(ERROR_MOD_NOT_FOUND);
				result = FALSE;
				break;
			}

			tmp = (Win::PE::HCUSTOMMODULE*)realloc(module->modules, (module->numModules + 1) * (sizeof(Win::PE::HCUSTOMMODULE)));
			if (tmp == NULL) {
				module->freeLibrary(handle, module->userdata);
				SetLastError(ERROR_OUTOFMEMORY);
				result = FALSE;
				break;
			}
			module->modules = tmp;

			module->modules[module->numModules++] = handle;
			if (importDesc->OriginalFirstThunk) {
				thunkRef = (uintptr_t*)(codeBase + importDesc->OriginalFirstThunk);
				funcRef = (FARPROC*)(codeBase + importDesc->FirstThunk);
			}
			else {
				// no hint table
				thunkRef = (uintptr_t*)(codeBase + importDesc->FirstThunk);
				funcRef = (FARPROC*)(codeBase + importDesc->FirstThunk);
			}
			for (; *thunkRef; thunkRef++, funcRef++) {
				if (IMAGE_SNAP_BY_ORDINAL(*thunkRef)) {
					*funcRef = module->getProcAddress(handle, (LPCSTR)IMAGE_ORDINAL(*thunkRef), module->userdata);
				}
				else {
					PIMAGE_IMPORT_BY_NAME thunkData = (PIMAGE_IMPORT_BY_NAME)(codeBase + (*thunkRef));
					*funcRef = module->getProcAddress(handle, (LPCSTR)&thunkData->Name, module->userdata);
				}
				if (*funcRef == 0) {
					result = FALSE;
					break;
				}
			}

			if (!result) {
				module->freeLibrary(handle, module->userdata);
				SetLastError(ERROR_PROC_NOT_FOUND);
				break;
			}
		}

		return result;
	}




	LPVOID Win::PE::MemoryDefaultAlloc(LPVOID address, SIZE_T size, DWORD allocationType, DWORD protect, void* userdata)
	{
		UNREFERENCED_PARAMETER(userdata);
		return VirtualAlloc(address, size, allocationType, protect);
	}

	BOOL Win::PE::MemoryDefaultFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType, void* userdata)
	{
		UNREFERENCED_PARAMETER(userdata);
		return VirtualFree(lpAddress, dwSize, dwFreeType);
	}

	Win::PE::HCUSTOMMODULE Win::PE::MemoryDefaultLoadLibrary(const char* filename, void* userdata)
	{
		HMODULE result;
		UNREFERENCED_PARAMETER(userdata);
		result = LoadLibraryA(filename);
		if (result == NULL) {
			return NULL;
		}
		return (Win::PE::HCUSTOMMODULE)result;
	}

	FARPROC Win::PE::MemoryDefaultGetProcAddress(Win::PE::HCUSTOMMODULE module, const char* name, void* userdata)
	{
		UNREFERENCED_PARAMETER(userdata);
		return (FARPROC)GetProcAddress((HMODULE)module, name);
	}

	void Win::PE::MemoryDefaultFreeLibrary(Win::PE::HCUSTOMMODULE module, void* userdata)
	{
		UNREFERENCED_PARAMETER(userdata);
		FreeLibrary((HMODULE)module);
	}

	Win::PE::HMEMORYMODULE Win::PE::MemoryLoadLibrary(const void* data, size_t size)
	{
		return MemoryLoadLibraryEx(
			data,
			size,
			Win::PE::MemoryDefaultAlloc,
			Win::PE::MemoryDefaultFree,
			Win::PE::MemoryDefaultLoadLibrary,
			Win::PE::MemoryDefaultGetProcAddress,
			Win::PE::MemoryDefaultFreeLibrary,
			NULL);
	}

	Win::PE::HMEMORYMODULE Win::PE::MemoryLoadLibraryEx(
		const void* data,
		size_t size,
		Win::PE::CustomAllocFunc allocMemory,
		Win::PE::CustomFreeFunc freeMemory,
		Win::PE::CustomLoadLibraryFunc loadLibrary,
		Win::PE::CustomGetProcAddressFunc getProcAddress,
		Win::PE::CustomFreeLibraryFunc freeLibrary,
		void* userdata)
	{
		bool load = false;
		bool success = false;
		PMEMORYMODULE result = NULL;
		PIMAGE_DOS_HEADER dos_header;
		PIMAGE_NT_HEADERS old_header;
		unsigned char* code, * headers;
		ptrdiff_t locationDelta;
		SYSTEM_INFO sysInfo;
		PIMAGE_SECTION_HEADER section;
		DWORD i;
		size_t optionalSectionSize;
		size_t lastSectionEnd = 0;
		size_t alignedImageSize;
#ifdef _WIN64
		POINTER_LIST* blockedMemory = NULL;
#endif
		do {
			if (!CheckSize(size, sizeof(IMAGE_DOS_HEADER)))
				break;
			dos_header = (PIMAGE_DOS_HEADER)data;
			if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
				break;
			if (!CheckSize(size, dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS)))
				break;
			old_header = (PIMAGE_NT_HEADERS) & ((const unsigned char*)(data))[dos_header->e_lfanew];
			if (old_header->Signature != IMAGE_NT_SIGNATURE)
				break;
			if (old_header->FileHeader.Machine != HOST_MACHINE)
				break;
			if (old_header->OptionalHeader.SectionAlignment & 1)
				break;
			section = IMAGE_FIRST_SECTION(old_header);
			optionalSectionSize = old_header->OptionalHeader.SectionAlignment;
			for (i = 0; i < old_header->FileHeader.NumberOfSections; i++, section++) {
				size_t endOfSection;
				if (section->SizeOfRawData == 0) {
					// Section without data in the DLL
					endOfSection = section->VirtualAddress + optionalSectionSize;
				}
				else {
					endOfSection = section->VirtualAddress + section->SizeOfRawData;
				}

				if (endOfSection > lastSectionEnd) {
					lastSectionEnd = endOfSection;
				}
			}
			::GetNativeSystemInfo(&sysInfo);
			alignedImageSize = AlignValueUp(old_header->OptionalHeader.SizeOfImage, sysInfo.dwPageSize);
			if (alignedImageSize != AlignValueUp(lastSectionEnd, sysInfo.dwPageSize))
				break;
			// reserve memory for image of library
			// XXX: is it correct to commit the complete memory region at once?
			//      calling DllEntry raises an exception if we don't...
			code = (unsigned char*)allocMemory((LPVOID)(old_header->OptionalHeader.ImageBase),
				alignedImageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE, userdata);

			if (code == NULL) {
				// try to allocate memory at arbitrary position
				code = (unsigned char*)allocMemory(NULL, alignedImageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE, userdata);
				if (code == NULL)
					break;
			}

#ifdef _WIN64
			// Memory block may not span 4 GB boundaries.
			while ((((uintptr_t)code) >> 32) < (((uintptr_t)(code + alignedImageSize)) >> 32)) {
				POINTER_LIST* node = (POINTER_LIST*)malloc(sizeof(POINTER_LIST));
				if (!node) {
					freeMemory(code, 0, MEM_RELEASE, userdata);
					FreePointerList(blockedMemory, freeMemory, userdata);
					break;
				}
				node->next = blockedMemory;
				node->address = code;
				blockedMemory = node;
				code = (unsigned char*)allocMemory(NULL, alignedImageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE, userdata);
				if (code == NULL) {
					FreePointerList(blockedMemory, freeMemory, userdata);
					break;
				}
			}
#endif

			result = (PMEMORYMODULE)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MEMORYMODULE));
			if (result == NULL) {
				freeMemory(code, 0, MEM_RELEASE, userdata);
#ifdef _WIN64
				FreePointerList(blockedMemory, freeMemory, userdata);
#endif
				break;
			}

			result->codeBase = code;
			result->isDLL = (old_header->FileHeader.Characteristics & IMAGE_FILE_DLL) != 0;
			result->alloc = allocMemory;
			result->free = freeMemory;
			result->loadLibrary = loadLibrary;
			result->getProcAddress = getProcAddress;
			result->freeLibrary = freeLibrary;
			result->userdata = userdata;
			result->pageSize = sysInfo.dwPageSize;
#ifdef _WIN64
			result->blockedMemory = blockedMemory;
#endif

			load = true;

			if (!CheckSize(size, old_header->OptionalHeader.SizeOfHeaders))
				break;

			// commit memory for headers
			headers = (unsigned char*)allocMemory(code, old_header->OptionalHeader.SizeOfHeaders, MEM_COMMIT, PAGE_READWRITE, userdata);
			// copy PE header to code
			memcpy(headers, dos_header, old_header->OptionalHeader.SizeOfHeaders);
			result->headers = (PIMAGE_NT_HEADERS) & ((const unsigned char*)(headers))[dos_header->e_lfanew];
			// update position
			result->headers->OptionalHeader.ImageBase = (uintptr_t)code;
			// copy sections from DLL file block to new memory location
			if (!CopySections((const unsigned char*)data, size, old_header, result))
				break;
			// adjust base address of imported data
			locationDelta = (ptrdiff_t)(result->headers->OptionalHeader.ImageBase - old_header->OptionalHeader.ImageBase);
			if (locationDelta != 0) {
				result->isRelocated = PerformBaseRelocation(result, locationDelta);
			}
			else {
				result->isRelocated = TRUE;
			}
			// load required dlls and adjust function table of imports
			if (!BuildImportTable(result))
				break;
			// mark memory pages depending on section headers and release
			// sections that are marked as "discardable"
			if (!FinalizeSections(result))
				break;
			// TLS callbacks are executed BEFORE the main loading
			if (!ExecuteTLS(result))
				break;
			// get entry point of loaded library
			if (result->headers->OptionalHeader.AddressOfEntryPoint != 0) {
				if (result->isDLL) {
					DllEntryProc DllEntry = (DllEntryProc)(LPVOID)(code + result->headers->OptionalHeader.AddressOfEntryPoint);
					// notify library about attaching to process
					BOOL successfull = (*DllEntry)((HINSTANCE)code, DLL_PROCESS_ATTACH, 0);
					if (!successfull)
						break;
					result->initialized = TRUE;
				}
				else {
					result->exeEntry = (ExeEntryProc)(LPVOID)(code + result->headers->OptionalHeader.AddressOfEntryPoint);
				}
			}
			else {
				result->exeEntry = NULL;
			}
			success = true;
		} while (0);
		if (result && load && !success) {
			MemoryFreeLibrary(result);
			result = NULL;
		}
		return (HMEMORYMODULE)result;
	}

	static int _compare(const void* a, const void* b)
	{
		const struct ExportNameEntry* p1 = (const struct ExportNameEntry*)a;
		const struct ExportNameEntry* p2 = (const struct ExportNameEntry*)b;
		return strcmp(p1->name, p2->name);
	}

	static int _find(const void* a, const void* b)
	{
		const char** name = (const char**)a;
		const struct ExportNameEntry* p = (const struct ExportNameEntry*)b;
		return strcmp(*name, p->name);
	}

	FARPROC Win::PE::MemoryGetProcAddress(HMEMORYMODULE mod, LPCSTR name)
	{
		FARPROC result = nullptr;
		do {
			PMEMORYMODULE __module = (PMEMORYMODULE)mod;
			if (!__module)
				break;
			unsigned char* codeBase = __module->codeBase;
			if (!codeBase)
				break;
			DWORD idx = 0;
			PIMAGE_EXPORT_DIRECTORY exports;
			PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY(__module, IMAGE_DIRECTORY_ENTRY_EXPORT);
			if (directory->Size == 0)
				break;
			exports = (PIMAGE_EXPORT_DIRECTORY)(codeBase + directory->VirtualAddress);
			if (exports->NumberOfNames == 0 || exports->NumberOfFunctions == 0)
				break;

			if (HIWORD(name) == 0) {
				// load function by ordinal value
				if (LOWORD(name) < exports->Base)
					break;
				idx = LOWORD(name) - exports->Base;
			}
			else if (!exports->NumberOfNames) {
				break;
			}
			else {
				// Lazily build name table and sort it by names
				if (!__module->nameExportsTable) {
					DWORD i;
					DWORD* nameRef = (DWORD*)(codeBase + exports->AddressOfNames);
					WORD* ordinal = (WORD*)(codeBase + exports->AddressOfNameOrdinals);
					struct ExportNameEntry* entry = (struct ExportNameEntry*)malloc(exports->NumberOfNames * sizeof(struct ExportNameEntry));
					__module->nameExportsTable = entry;
					if (!entry)
						break;
					for (i = 0; i < exports->NumberOfNames; i++, nameRef++, ordinal++, entry++) {
						entry->name = (const char*)(codeBase + (*nameRef));
						entry->idx = *ordinal;
					}
					::qsort(__module->nameExportsTable, exports->NumberOfNames, sizeof(struct ExportNameEntry), _compare);
				}
				// search function name in list of exported names with binary search
				ExportNameEntry* pFoundExportNameEntry = nullptr;
				pFoundExportNameEntry = reinterpret_cast<ExportNameEntry*>(
					::bsearch(&name, __module->nameExportsTable, exports->NumberOfNames, sizeof(ExportNameEntry), _find));
				if (!pFoundExportNameEntry)
					break;
				idx = pFoundExportNameEntry->idx;
			}
			if (idx > exports->NumberOfFunctions)
				break;
			// AddressOfFunctions contains the RVAs to the "real" functions
			result = reinterpret_cast<FARPROC>(codeBase + (*(DWORD*)(codeBase + exports->AddressOfFunctions + (idx * 4))));
		} while (0);
		return result;
	}

	void Win::PE::MemoryFreeLibrary(HMEMORYMODULE mod) {
		PMEMORYMODULE __module = (PMEMORYMODULE)mod;
		do {
			if (!__module)
				break;
			if (__module->initialized) {
				// notify library about detaching from process
				DllEntryProc DllEntry = (DllEntryProc)(LPVOID)(__module->codeBase + __module->headers->OptionalHeader.AddressOfEntryPoint);
				(*DllEntry)((HINSTANCE)__module->codeBase, DLL_PROCESS_DETACH, 0);
			}
			::free(__module->nameExportsTable);
			if (__module->modules != NULL) {
				// free previously opened libraries
				for (int i = 0; i < __module->numModules; i++) {
					if (__module->modules[i] != NULL) {
						__module->freeLibrary(__module->modules[i], __module->userdata);
					}
				}
				::free(__module->modules);
			}
			if (__module->codeBase != NULL) {
				// release memory of library
				__module->free(__module->codeBase, 0, MEM_RELEASE, __module->userdata);
			}
#ifdef _WIN64
			FreePointerList(__module->blockedMemory, __module->free, __module->userdata);
#endif
			::HeapFree(::GetProcessHeap(), 0, __module);
		} while (0);
	}

	int Win::PE::MemoryCallEntryPoint(Win::PE::HMEMORYMODULE mod) {
		int result = -1;
		PMEMORYMODULE __module = (PMEMORYMODULE)mod;
		do {
			if (__module == NULL || __module->isDLL || __module->exeEntry == NULL || !__module->isRelocated)
				break;
			result = __module->exeEntry();
		} while (0);
		return result;
	}

#define DEFAULT_LANGUAGE        MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)

	Win::PE::HMEMORYRSRC Win::PE::MemoryFindResource(Win::PE::HMEMORYMODULE module, LPCTSTR name, LPCTSTR type)
	{
		return MemoryFindResourceEx(module, name, type, DEFAULT_LANGUAGE);
	}

	static PIMAGE_RESOURCE_DIRECTORY_ENTRY _MemorySearchResourceEntry(
		void* root,
		PIMAGE_RESOURCE_DIRECTORY resources,
		LPCTSTR key)
	{
		PIMAGE_RESOURCE_DIRECTORY_ENTRY entries = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(resources + 1);
		PIMAGE_RESOURCE_DIRECTORY_ENTRY result = NULL;
		DWORD start;
		DWORD end;
		DWORD middle;

		if (!IS_INTRESOURCE(key) && key[0] == TEXT('#')) {
			// special case: resource id given as string
			TCHAR* endpos = NULL;
			long int tmpkey = (WORD)wcstol((TCHAR*)&key[1], &endpos, 10);
			if (tmpkey <= 0xffff && lstrlen(endpos) == 0) {
				key = MAKEINTRESOURCE(tmpkey);
			}
		}

		// entries are stored as ordered list of named entries,
		// followed by an ordered list of id entries - we can do
		// a binary search to find faster...
		if (IS_INTRESOURCE(key)) {
			WORD check = (WORD)(uintptr_t)key;
			start = resources->NumberOfNamedEntries;
			end = start + resources->NumberOfIdEntries;

			while (end > start) {
				WORD entryName;
				middle = (start + end) >> 1;
				entryName = (WORD)entries[middle].Name;
				if (check < entryName) {
					end = (end != middle ? middle : middle - 1);
				}
				else if (check > entryName) {
					start = (start != middle ? middle : middle + 1);
				}
				else {
					result = &entries[middle];
					break;
				}
			}
		}
		else {
			LPCWSTR searchKey;
			size_t searchKeyLen = wcslen(key);
#if defined(UNICODE)
			searchKey = key;
#else
			// Resource names are always stored using 16bit characters, need to
			// convert string we search for.
#define MAX_LOCAL_KEY_LENGTH 2048
								// In most cases resource names are short, so optimize for that by
								// using a pre-allocated array.
			wchar_t _searchKeySpace[MAX_LOCAL_KEY_LENGTH + 1];
			LPWSTR _searchKey;
			if (searchKeyLen > MAX_LOCAL_KEY_LENGTH) {
				size_t _searchKeySize = (searchKeyLen + 1) * sizeof(wchar_t);
				_searchKey = (LPWSTR)malloc(_searchKeySize);
				if (_searchKey == NULL) {
					SetLastError(ERROR_OUTOFMEMORY);
					return NULL;
				}
			}
			else {
				_searchKey = &_searchKeySpace[0];
			}

			mbstowcs(_searchKey, key, searchKeyLen);
			_searchKey[searchKeyLen] = 0;
			searchKey = _searchKey;
#endif
			start = 0;
			end = resources->NumberOfNamedEntries;
			while (end > start) {
				int cmp;
				PIMAGE_RESOURCE_DIR_STRING_U resourceString;
				middle = (start + end) >> 1;
				resourceString = (PIMAGE_RESOURCE_DIR_STRING_U)OffsetPointer(root, entries[middle].Name & 0x7FFFFFFF);
				cmp = _wcsnicmp(searchKey, resourceString->NameString, resourceString->Length);
				if (cmp == 0) {
					// Handle partial match
					if (searchKeyLen > resourceString->Length) {
						cmp = 1;
					}
					else if (searchKeyLen < resourceString->Length) {
						cmp = -1;
					}
				}
				if (cmp < 0) {
					end = (middle != end ? middle : middle - 1);
				}
				else if (cmp > 0) {
					start = (middle != start ? middle : middle + 1);
				}
				else {
					result = &entries[middle];
					break;
				}
			}
#if !defined(UNICODE)
			if (searchKeyLen > MAX_LOCAL_KEY_LENGTH) {
				free(_searchKey);
			}
#undef MAX_LOCAL_KEY_LENGTH
#endif
		}

		return result;
	}

	Win::PE::HMEMORYRSRC Win::PE::MemoryFindResourceEx(Win::PE::HMEMORYMODULE module, LPCTSTR name, LPCTSTR type, WORD language)
	{
		unsigned char* codeBase = ((PMEMORYMODULE)module)->codeBase;
		PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY((PMEMORYMODULE)module, IMAGE_DIRECTORY_ENTRY_RESOURCE);
		PIMAGE_RESOURCE_DIRECTORY rootResources;
		PIMAGE_RESOURCE_DIRECTORY nameResources;
		PIMAGE_RESOURCE_DIRECTORY typeResources;
		PIMAGE_RESOURCE_DIRECTORY_ENTRY foundType;
		PIMAGE_RESOURCE_DIRECTORY_ENTRY foundName;
		PIMAGE_RESOURCE_DIRECTORY_ENTRY foundLanguage;
		if (directory->Size == 0) {
			// no resource table found
			SetLastError(ERROR_RESOURCE_DATA_NOT_FOUND);
			return NULL;
		}

		if (language == DEFAULT_LANGUAGE) {
			// use language from current thread
			language = LANGIDFROMLCID(GetThreadLocale());
		}

		// resources are stored as three-level tree
		// - first node is the type
		// - second node is the name
		// - third node is the language
		rootResources = (PIMAGE_RESOURCE_DIRECTORY)(codeBase + directory->VirtualAddress);
		foundType = _MemorySearchResourceEntry(rootResources, rootResources, type);
		if (foundType == NULL) {
			SetLastError(ERROR_RESOURCE_TYPE_NOT_FOUND);
			return NULL;
		}

		typeResources = (PIMAGE_RESOURCE_DIRECTORY)(codeBase + directory->VirtualAddress + (foundType->OffsetToData & 0x7fffffff));
		foundName = _MemorySearchResourceEntry(rootResources, typeResources, name);
		if (foundName == NULL) {
			SetLastError(ERROR_RESOURCE_NAME_NOT_FOUND);
			return NULL;
		}

		nameResources = (PIMAGE_RESOURCE_DIRECTORY)(codeBase + directory->VirtualAddress + (foundName->OffsetToData & 0x7fffffff));
		foundLanguage = _MemorySearchResourceEntry(rootResources, nameResources, (LPCTSTR)(uintptr_t)language);
		if (foundLanguage == NULL) {
			// requested language not found, use first available
			if (nameResources->NumberOfIdEntries == 0) {
				SetLastError(ERROR_RESOURCE_LANG_NOT_FOUND);
				return NULL;
			}

			foundLanguage = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(nameResources + 1);
		}

		return (codeBase + directory->VirtualAddress + (foundLanguage->OffsetToData & 0x7fffffff));
	}

	DWORD Win::PE::MemorySizeofResource(Win::PE::HMEMORYMODULE module, Win::PE::HMEMORYRSRC resource)
	{
		PIMAGE_RESOURCE_DATA_ENTRY entry;
		UNREFERENCED_PARAMETER(module);
		entry = (PIMAGE_RESOURCE_DATA_ENTRY)resource;
		if (entry == NULL) {
			return 0;
		}

		return entry->Size;
	}

	LPVOID Win::PE::MemoryLoadResource(Win::PE::HMEMORYMODULE module, Win::PE::HMEMORYRSRC resource)
	{
		unsigned char* codeBase = ((PMEMORYMODULE)module)->codeBase;
		PIMAGE_RESOURCE_DATA_ENTRY entry = (PIMAGE_RESOURCE_DATA_ENTRY)resource;
		if (entry == NULL) {
			return NULL;
		}

		return codeBase + entry->OffsetToData;
	}

	int
		Win::PE::MemoryLoadString(Win::PE::HMEMORYMODULE module, UINT id, LPTSTR buffer, int maxsize)
	{
		return MemoryLoadStringEx(module, id, buffer, maxsize, DEFAULT_LANGUAGE);
	}

	int
		Win::PE::MemoryLoadStringEx(Win::PE::HMEMORYMODULE module, UINT id, LPTSTR buffer, int maxsize, WORD language)
	{
		HMEMORYRSRC resource;
		PIMAGE_RESOURCE_DIR_STRING_U data;
		DWORD size;
		if (maxsize == 0) {
			return 0;
		}

		resource = MemoryFindResourceEx(module, MAKEINTRESOURCE((id >> 4) + 1), RT_STRING, language);
		if (resource == NULL) {
			buffer[0] = 0;
			return 0;
		}

		data = (PIMAGE_RESOURCE_DIR_STRING_U)MemoryLoadResource(module, resource);
		id = id & 0x0f;
		while (id--) {
			data = (PIMAGE_RESOURCE_DIR_STRING_U)OffsetPointer(data, (data->Length + 1) * sizeof(WCHAR));
		}
		if (data->Length == 0) {
			SetLastError(ERROR_RESOURCE_NAME_NOT_FOUND);
			buffer[0] = 0;
			return 0;
		}

		size = data->Length;
		if (size >= (DWORD)maxsize) {
			size = maxsize;
		}
		else {
			buffer[size] = 0;
		}
#if defined(UNICODE)
		wcsncpy_s(buffer, size, data->NameString, size);
#else
		wcstombs(buffer, data->NameString, size);
#endif
		return size;
	}

	inline std::string Win::GetModulePathA(const HINSTANCE& hModule /*= nullptr*/) {
		std::string result;
		do {
			char Filename[MAX_PATH] = { 0 };
			if (::GetModuleFileNameA(hModule, Filename, MAX_PATH) <= 0)
				break;
			if (FALSE == ::PathRemoveFileSpecA(Filename))
				break;
			result = Filename;
			auto end = std::prev(result.end());
			if (*end != '\\' && *end != '/')
				result.push_back('\\');
		} while (0);
		return result;
	}
	inline std::wstring Win::GetModulePathW(const HINSTANCE& hModule) {
		std::wstring result;
		do {
			wchar_t Filename[MAX_PATH] = { 0 };
			if (::GetModuleFileNameW(hModule, Filename, MAX_PATH) <= 0)
				break;
			if (FALSE == ::PathRemoveFileSpecW(Filename))
				break;
			result = Filename;
			auto end = std::prev(result.end());
			if (*end != '\\' && *end != '/')
				result.push_back('\\');
		} while (0);
		return result;
	}
	inline std::string Win::GetModulePathnameA(const HINSTANCE& hModule) {
		std::string result;
		do {
			char Filename[MAX_PATH] = { 0 };
			if (::GetModuleFileNameA(hModule, Filename, MAX_PATH) <= 0)
				break;
			result = Filename;
		} while (0);
		return result;
	}
	inline std::wstring Win::GetModulePathnameW(const HINSTANCE& hModule) {
		std::wstring result;
		do {
			wchar_t Filename[MAX_PATH] = { 0 };
			if (::GetModuleFileNameW(hModule, Filename, MAX_PATH) <= 0)
				break;
			result = Filename;
		} while (0);
		return result;
	}
	inline std::string Win::EnumCorrectPath(const std::string& in, const char& replace_at /*= '\\'*/, const bool& isFolder /*= true*/) {
		std::string result{ in };
		if (result.empty())
			return result;
		do {
			auto found = result.find(replace_at == '\\' ? '/' : '\\');
			if (found != std::string::npos) {
				result[found] = replace_at;
				continue;
			}
			found = result.find("\\/");
			if (found != std::string::npos) {
				result.replace(found, strlen("\\/"), sizeof(replace_at), replace_at);
				continue;
			}
			found = result.find("/\\");
			if (found != std::string::npos) {
				result.replace(found, strlen("/\\"), sizeof(replace_at), replace_at);
				continue;
			}
			found = result.find(replace_at == '\\' ? "\\\\" : "//");
			if (found != std::string::npos) {
				result.replace(found, strlen(replace_at == '\\' ? "//" : "\\\\"), sizeof(replace_at), replace_at);
				continue;
			}
			found = result.find(replace_at == '\\' ? "\\\\\\" : "///");
			if (found != std::string::npos) {
				result.replace(found, strlen(replace_at == '\\' ? "\\\\\\" : "///"), sizeof(replace_at), replace_at);
				continue;
			}
			found = result.find(replace_at == '\\' ? "\\\\\\\\" : "////");
			if (found != std::string::npos) {
				result.replace(found, strlen(replace_at == '\\' ? "\\\\\\\\" : "////"), sizeof(replace_at), replace_at);
				continue;
			}
			found = result.find({ replace_at ,replace_at });
			if (found != std::string::npos) {
				result.replace(found, sizeof(replace_at) * 2, sizeof(replace_at), replace_at);
				continue;
			}
			found = result.find({ replace_at ,replace_at,replace_at });
			if (found != std::string::npos) {
				result.replace(found, sizeof(replace_at) * 3, sizeof(replace_at), replace_at);
				continue;
			}
			break;
		} while (1);
		do {
			if (isFolder) {
				if (result.empty()) {
					result.append(sizeof(replace_at), replace_at);
					break;
				}
				if (*std::prev(result.end()) != replace_at) {
					result.push_back(replace_at);
				}
			}
			else {
				if (*result.begin() == replace_at) {
					result.erase(0, sizeof(replace_at));
				}
			}
		} while (0);
		return result;
	}
	inline void Win::EnumFoldersAndFiles(
		const std::string& Path,
		tfEnumFolderNode& Folders,
		tfEnumFolderNode& Files,
		const char* FileFilter /*= "*.*"*/,
		bool bSleepDirect /*= false*/,
		const std::function<void(const std::string&, const std::string&, const _finddata_t&)>& enumcb /*= nullptr*/) {
		if (Path.empty())
			return;
		std::vector<std::string> Paths{ Path };
		std::intptr_t firstIndes{ -1 };
		std::string path_pos;
		std::set<std::string> havedone;
		do {
			if (Paths.empty())
				break;
			auto begin = Paths.begin();
			std::string TargetPath(*begin);
			Paths.erase(begin);
			if (*std::prev(TargetPath.end()) != '\\' && *std::prev(TargetPath.end()) != '/') {
				TargetPath.push_back('\\');
			}
			path_pos = TargetPath;
			havedone.insert(path_pos);
			TargetPath.append(FileFilter);
			_finddata_t  FindData = { 0 };
			firstIndes = _findfirst(TargetPath.c_str(), &FindData);
			do {
				if (firstIndes < 0)
					break;
				bool _break = false;
				do {
					if (!::strcmp(FindData.name, ".") || !::strcmp(FindData.name, ".."))
						continue;
					if (bSleepDirect) {
						if (FindData.attrib & _A_SUBDIR) {
							auto the = path_pos + "\\" + FindData.name + "\\";
							if (havedone.find(the) == havedone.end()) {
								Paths.emplace_back(the);
								the.erase(the.find(Path), Path.size());
								Folders.emplace(std::make_pair(EnumCorrectPath(the), FindData.name));
								if (enumcb) {
									enumcb(the, FindData.name, FindData);
								}
								break;
							}
							else
								_break = true;
						}
						else {
							auto the = path_pos + "\\" + FindData.name;
							the.erase(the.find(Path), Path.size());
							Files.emplace(std::make_pair(EnumCorrectPath(the, '\\', false), FindData.name));
							if (enumcb) {
								enumcb(the, FindData.name, FindData);
							}
						}
						break;
					}
					else {
						if (FindData.attrib & _A_SUBDIR) {
							auto the = path_pos + "\\" + FindData.name + "\\";
							the.erase(the.find(Path), Path.size());
							Folders.emplace(std::make_pair(EnumCorrectPath(the), FindData.name));
							if (enumcb) {
								enumcb(the, FindData.name, FindData);
							}
							break;
						}
						auto the = path_pos + "\\" + FindData.name;
						the.erase(the.find(Path), Path.size());
						Files.emplace(std::make_pair(EnumCorrectPath(the, '\\', false), FindData.name));
						if (enumcb) {
							enumcb(the, FindData.name, FindData);
						}
					}
				} while (0);
				if (_break)
					break;
				::memset(&FindData, 0x00, sizeof(FindData));
				if (_findnext(firstIndes, &FindData) < 0)
					break;
			} while (1);
		} while (1);
		if (firstIndes >= 0) {
			::_findclose(firstIndes);
		}
	}

	inline void Win::File::ParseA(const std::string& inStrContend, const int& nDelimiter, std::vector<std::string>& Output) {
		Output.clear();
		do {
			if (inStrContend.empty())
				break;
			std::string tempContent{ inStrContend };
			size_t found = 0;
			while ((found = tempContent.find(nDelimiter)) != std::string::npos) {
				std::string strSession;
				strSession.insert(strSession.length(), tempContent.data(), found);
				tempContent.erase(0, found + 1);
				Output.push_back(strSession);
			}
			if (tempContent.empty())
				break;
			Output.push_back(tempContent);
		} while (0);
	}
	inline std::string Win::File::Read(const std::string& FilePathname, const std::string& OpenMode /*= "rb"*/, const int& _ShFlag /*= _SH_DENYNO*/) {
		std::string result;
		FILE* pFile = nullptr;
		do {
			pFile = _fsopen(FilePathname.c_str(), OpenMode.c_str(), _ShFlag);
			if (!pFile)
				break;
			if (0 != fseek(pFile, 0, SEEK_END))
				break;
			long lSize = ftell(pFile);
			if (lSize <= 0)
				break;
			rewind(pFile);
			//::fseek(pFile, 0, SEEK_SET);
			result.resize(lSize);
			fread(&result[0], 1, lSize, pFile);
		} while (0);
		if (pFile) {
			fclose(pFile);
			pFile = nullptr;
		}
		return result;
	}

	inline std::string memade::Win::Json::to_string(const rapidjson::Value& valObj) {
		std::string result;
		rapidjson::StringBuffer jbuffer;
		rapidjson::Writer<rapidjson::StringBuffer> jwriter(jbuffer);
		if (valObj.Accept(jwriter))
			result = std::string(jbuffer.GetString(), jbuffer.GetLength());
		return result;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline PluginModule::PluginModule(const std::string& plugin_pepathname) :
		m_plugin_pepathname(plugin_pepathname) {
		Init();
	}
	inline PluginModule::~PluginModule() {
		UnInit();
	}
	inline void PluginModule::Release() const {
		delete this;
	}
	inline void PluginModule::Init() {
		do {
			if (m_plugin_pepathname.empty())
				break;
			auto pe_buffer = Win::File::Read(m_plugin_pepathname);
			if (pe_buffer.empty())
				break;
			m_module_handle = Win::PE::MemoryLoadLibrary(pe_buffer.data(), pe_buffer.size());
			if (!m_module_handle)
				break;
			m_module_init = reinterpret_cast<decltype(m_module_init)>\
				(Win::PE::MemoryGetProcAddress(m_module_handle, MEMADE_PLUGIN_FUNCTION_INIT));
			m_module_uninit = reinterpret_cast<decltype(m_module_uninit)>\
				(Win::PE::MemoryGetProcAddress(m_module_handle, MEMADE_PLUGIN_FUNCTION_UNINIT));
			if (!m_module_handle || !m_module_init || !m_module_uninit)
				break;
			void* route = nullptr;
			unsigned long route_size = 0;
			m_module_init(&route, route_size);
			if (route && route_size > 0) {
				std::vector<std::string> funs;
				Win::File::ParseA(std::string((char*)route, route_size), ',', funs);
				for (const auto& node : funs) {
					tf_memade_plugin_common_function f = \
						(tf_memade_plugin_common_function)Win::PE::MemoryGetProcAddress(m_module_handle, node.data());
					if (f)
						m_plugin_function_s.emplace(node, f);
				}
				MEMADE_FREE_ROUTE(route);
			}
			m_ready = true;
		} while (0);
	}
	inline void PluginModule::UnInit() {
		if (m_ready) {
			m_module_uninit();
			Win::PE::MemoryFreeLibrary(m_module_handle);
			m_ready = false;
		}
	}
	inline void* PluginModule::Identify() const {
		return m_module_handle;
	}
	inline bool PluginModule::Ready() const {
		return m_ready;
	}
	inline tf_memade_plugin_common_function PluginModule::Search(const std::string& function_name) const {
		tf_memade_plugin_common_function found = nullptr;
		do {
			if (function_name.empty())
				break;
			auto find = m_plugin_function_s.find(function_name);
			if (find == m_plugin_function_s.end())
				break;
			found = find->second;
		} while (0);
		return found;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline Chromium::Chromium() {

	}
	inline Chromium::~Chromium() {

	}
	inline const DWORD& Chromium::CurrentParentProcessId() const {
		return m_CurrentParentProcessId;
	}
	inline void Chromium::Init() {
		m_CurrentParentProcessId = \
			base::GetParentProcessId(base::GetCurrentProcessHandle());
		do {//!@ init plugin modules
			std::string plugins_path;
#if _DEBUG
			plugins_path = R"(D:\github\Windows\projects\browser_plugins\bin\x64\Release\)";
#else
			plugins_path = Win::GetModulePathA() + R"(\plugins\)";
#endif
			tfEnumFolderNode enum_files, enum_folders;
			Win::EnumFoldersAndFiles(plugins_path, enum_folders, enum_files, "*.dll");
			if (enum_files.empty())
				break;
			for (const auto& pe_node : enum_files) {
				const std::string current_pathanme = plugins_path + pe_node.second;
				do {
					PluginModule* pluginObj = new PluginModule(current_pathanme);
					if (!pluginObj->Ready()) {
						pluginObj->Release();
						break;
					}
					m_plugin_module_s.emplace(pluginObj->Identify(), pluginObj);
				} while (0);
			}

			if (plugins_path.empty())
				return;
		} while (0);
	}
	inline void Chromium::UnInit() {
		for (auto& node : m_plugin_module_s)
			node.second->Release();
	}
	inline tf_memade_plugin_common_function Chromium::FindPluginFunction(const std::string& function_name) const {
		tf_memade_plugin_common_function result = nullptr;
		std::lock_guard<std::mutex> lock{ *m_Mutex };
		do {
			if (function_name.empty())
				break;
			if (m_plugin_module_s.empty())
				break;
			for (const auto& node : m_plugin_module_s) {
				if ((result = node.second->Search(function_name)) != nullptr)
					break;
			}
		} while (0);
		return result;
	}




}///namespace memade


extern "C" {
	//!@ in struct OFFLINEAUDIOCONTEXTROUTE
	bool __stdcall MemadeCommonHookOnLocal(void** route, size_t& route_size, const void* view, const char* remote_function) {
		bool result = false;
		do {
			if (!remote_function)
				break;
			auto find_function = memade::__gspChromium->FindPluginFunction(remote_function);
			if (!find_function)
				break;
			result = find_function(route, route_size, view);
		} while (0);
		return result;
	}

}///extern "C"

#endif///#if MEMADE_BROWSER_PROCESS_CHEILD_HOOK