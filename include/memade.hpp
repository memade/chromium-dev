#if !defined(__0AA9550E_7EC1_4E2D_884E_D11F1CD8FD72__)
#define __0AA9550E_7EC1_4E2D_884E_D11F1CD8FD72__

#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <io.h>
#include <memory>
#include <mutex>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <format>

#include "rapidjson.hpp"
#include "peload.hpp"
#include "windows.hpp"
#include "chromium.hpp"

namespace memade {
	using tf_plugin_api_object_init = void(__stdcall*)(void**, unsigned long&);
	using tf_plugin_api_object_uninit = void(__stdcall*)(void);
	using tf_plugin_common_api = bool(__stdcall*)(void**, unsigned long&, const void*);
	static const char gc_plugin_function_sign_init[] = "api_object_init";
	static const char gc_plugin_function_sign_uninit[] = "api_object_uninit";

	class Plugin {
	public:
		inline Plugin(const std::string&);
		inline ~Plugin();
		inline void Release() const;
		inline const std::uintptr_t& Identify() const;
		inline const bool& Ready() const;
		inline tf_plugin_common_api Search(const std::string&) const;
	private:
		inline void Init();
		inline void UnInit();
	private:
		bool m_ready = false;
		std::uintptr_t m_identify = 0;
		std::string m_plugin_pepathname;
		std::map<std::string, tf_plugin_common_api> m_common_plugin_s;
		PE::HMEMORYMODULE m_module_handle = nullptr;
		tf_plugin_api_object_init m_api_object_init = nullptr;
		tf_plugin_api_object_uninit m_api_object_uninit = nullptr;
	};
	inline Plugin::Plugin(const std::string& plugin_pepathname) :
		m_plugin_pepathname(plugin_pepathname) {
		Init();
	}
	inline Plugin::~Plugin() {
		UnInit();
	}
	inline void Plugin::Release() const {
		delete this;
	}
	inline void Plugin::Init() {
		do {
			if (!Win::AccessA(m_plugin_pepathname))
				break;
			auto pe_buffer = Win::Read(m_plugin_pepathname);
			if (pe_buffer.empty())
				break;
			m_module_handle = PE::MemoryLoadLibrary(pe_buffer.data(), pe_buffer.size());
			if (!m_module_handle)
				break;
			m_identify = reinterpret_cast<decltype(m_identify)>(m_module_handle);
			m_api_object_init = reinterpret_cast<decltype(m_api_object_init)>\
				(PE::MemoryGetProcAddress(m_module_handle, gc_plugin_function_sign_init));
			m_api_object_uninit = reinterpret_cast<decltype(m_api_object_uninit)>\
				(PE::MemoryGetProcAddress(m_module_handle, gc_plugin_function_sign_uninit));
			if (!m_module_handle || !m_api_object_init || !m_api_object_uninit)
				break;
			void* route = nullptr;
			unsigned long route_size = 0;
			m_api_object_init(&route, route_size);
			if (route && route_size > 0) {
				std::vector<std::string> funs;
				Win::SplitStringA(std::string((char*)route, route_size), ',', funs);
				for (const auto& node : funs) {
					tf_plugin_common_api fcomm = \
						reinterpret_cast<tf_plugin_common_api>(PE::MemoryGetProcAddress(m_module_handle, node.data()));
					if (fcomm)
						m_common_plugin_s.emplace(node, fcomm);
				}
			}
			MEMADE_ROUTE_FREE(route);
			m_ready = true;
		} while (0);
	}
	inline void Plugin::UnInit() {
		if (m_ready) {
			m_api_object_uninit();
			PE::MemoryFreeLibrary(m_module_handle);
			m_ready = false;
		}
	}
	inline const std::uintptr_t& Plugin::Identify() const {
		return m_identify;
	}
	inline const bool& Plugin::Ready() const {
		return m_ready;
	}
	inline tf_plugin_common_api
		Plugin::Search(const std::string& function_name) const {
		tf_plugin_common_api found = nullptr;
		do {
			if (function_name.empty())
				break;
			auto find = m_common_plugin_s.find(function_name);
			if (find == m_common_plugin_s.end())
				break;
			found = find->second;
		} while (0);
		return found;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Projects final {
		std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
	public:
		inline Projects();
		inline	~Projects();
		inline void Release() const;
	public:
		inline void Init();
		inline tf_plugin_common_api FindPluginApi(const std::string&) const;
	private:
		inline void UnInit();
	private:
		std::map<std::uintptr_t, Plugin*> m_plugin_s;
	};

	inline Projects::Projects() {

	}
	inline Projects::~Projects() {
		UnInit();
	}
	inline void Projects::Release() const {
		delete this;
	}
	inline void Projects::Init() {
		/*m_CurrentParentProcessId = \
			base::GetParentProcessId(base::GetCurrentProcessHandle());*/
		do {//!@ init plugin modules
			std::string plugins_path;
#if _DEBUG
			plugins_path = R"(D:\github\Windows\projects\browser_plugins\bin\x64\Release\)";
#else
			plugins_path = Win::GetModulePathA() + R"(\plugins\)";
#endif
			Win::tfEnumFolderNode enum_files, enum_folders;
			Win::EnumFoldersAndFilesA(plugins_path, enum_folders, enum_files, "*.dll");
			if (enum_files.empty())
				break;
			for (const auto& pe_node : enum_files) {
				const std::string current_pathanme = plugins_path + pe_node.second;
				do {
					Plugin* pluginObj = new Plugin(current_pathanme);
					if (!pluginObj->Ready()) {
						pluginObj->Release();
						break;
					}
					m_plugin_s.emplace(pluginObj->Identify(), pluginObj);
				} while (0);
			}


		} while (0);
	}
	inline void Projects::UnInit() {
		for (auto& node : m_plugin_s)
			node.second->Release();
	}
	inline tf_plugin_common_api Projects::FindPluginApi(const std::string& plugin_identify) const {
		tf_plugin_common_api result = nullptr;
		std::lock_guard<std::mutex> lock{ *m_Mutex };
		do {
			if (plugin_identify.empty())
				break;


		} while (0);
		return result;
	}


	static Projects* __gpsProjects = nullptr;
}///namespace memade










#define MEMADE_PROJECTS_INIT \
if(!memade::__gpsProjects){\
memade::__gpsProjects = new memade::Projects();\
memade::__gpsProjects->Init();\
}\

#define MEMADE_PROJECTS_UNINIT \
if(memade::__gpsProjects){\
memade::__gpsProjects->Release();\
}\










/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 01 Mar 2023 22:26:13 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__0AA9550E_7EC1_4E2D_884E_D11F1CD8FD72__


#if 0
#include <Windows.h>
#define MEMADE_MAIN_SWITCH 1
#define MEMADE_FINGERPRINT_FAKE 1
#define MEMADE_BROWSER_PROCESS_MAIN_HOOK 1
#define MEMADE_BROWSER_PROCESS_CHEILD_HOOK 1
#define MEMADE_BROWSER_PROCESS_MAIN_HOOK_MODULE_NAME L"MAIN.DLL"
#define MEMADE_BROWSER_PROCESS_CHILD_HOOK_MODULE_NAME L"CHROME_ELF.DLL"
#define MEMADE_LOCAL_HOOK_FUNCTION_NAME "MemadeCommonHookOnLocal"
#define MEMADE_PLUGIN_FUNCTION_INIT "api_object_init"
#define MEMADE_PLUGIN_FUNCTION_UNINIT "api_object_uninit"

#define MEMADE_VNAME(value) (#value)

#define MEMADE_FREE_ROUTE(p)\
do {\
if (!p)\
break;\
if (FALSE == ::HeapFree(::GetProcessHeap(), 0, p)) \
break;\
p = nullptr;\
} while (0);\

using tf_memade_plugin_init = void(__stdcall*)(void**, unsigned long&);
using tf_memade_plugin_uninit = void(__stdcall*)(void);
using tf_memade_plugin_common_function = bool(__stdcall*)(void**, size_t&, const void*);

using tfMemadeCommonHookOnLocal = bool(__stdcall*)(void**, size_t&, const void*, const char*);
using tfMemadeCommonHookOnRemote = tf_memade_plugin_common_function;

#define MEMADE_LOADLIBRARY(hModule,dllfname)\
do {\
hModule = ::GetModuleHandleW(MEMADE_BROWSER_PROCESS_MAIN_HOOK_MODULE_NAME);\
if (!hModule){hModule = LoadLibraryW(\
dllfname!=nullptr?\
(LPCWSTR)dllfname:\
(LPCWSTR)MEMADE_BROWSER_PROCESS_MAIN_HOOK_MODULE_NAME); }\
if (!hModule){	break;}\
auto plugin_init =(tf_memade_plugin_init)::GetProcAddress(hModule, MEMADE_PLUGIN_FUNCTION_INIT); \
void* route = nullptr;\
unsigned long route_size = 0;\
if (plugin_init){plugin_init(&route, route_size);}\
} while (0); \

#define MEMADE_UNLOADLIBRARY(hModule)\
do {\
if (!hModule){break;}\
auto plugin_uninit =(tf_memade_plugin_uninit)::GetProcAddress(hModule, MEMADE_PLUGIN_FUNCTION_UNINIT); \
if (plugin_uninit){plugin_uninit();}\
::FreeLibrary(hModule);\
hModule = nullptr;\
} while (0);\

namespace memade {
#pragma pack(push,1)
	//number_of_channels, number_of_frames, sample_rate
	//!@ MakeGarbageCollected<OfflineAudioContext>
	typedef struct tagOFFLINEAUDIOCONTEXTROUTE {
		unsigned int current_parent_process_id;
		unsigned int number_of_channels;
		unsigned int number_of_frames;
		float sample_rate;
		char request_url[2048];

		tagOFFLINEAUDIOCONTEXTROUTE() { ::memset(this, 0x00, sizeof(*this)); }
		void operator=(const tagOFFLINEAUDIOCONTEXTROUTE& obj) { ::memcpy(this, &obj, sizeof(*this)); }
	}OFFLINEAUDIOCONTEXTROUTE, * POFFLINEAUDIOCONTEXTROUTE;
#pragma pack(pop)

	template <typename TypeFunction = tfMemadeCommonHookOnLocal>
	bool TCommonHook(
		void** route,
		size_t& route_size,
		const void* view,
		const char* remote_hook_function_name) {
		bool result = false;
#if MEMADE_BROWSER_PROCESS_CHEILD_HOOK
		do {
			if (!remote_hook_function_name)
				break;
			HMODULE hModule = ::GetModuleHandleW(MEMADE_BROWSER_PROCESS_CHILD_HOOK_MODULE_NAME);
			if (!hModule)
				break;
			auto local_hook_function = reinterpret_cast<tfMemadeCommonHookOnLocal>(::GetProcAddress(hModule, MEMADE_LOCAL_HOOK_FUNCTION_NAME));
			if (!local_hook_function)
				break;
			result = local_hook_function(route, route_size, view, remote_hook_function_name);
		} while (0);
#endif///MEMADE_BROWSER_PROCESS_CHEILD_HOOK
		return result;
	}

}///namespace memade
#endif