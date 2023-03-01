#if !defined(__314894CF_D5D4_4D34_9558_18AECA81498F__)
#define __314894CF_D5D4_4D34_9558_18AECA81498F__

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





#if MEMADE_MAIN_SWITCH && MEMADE_BROWSER_PROCESS_MAIN_HOOK
extern HMODULE __hMemadeBrowserMain;
#endif///#if MEMADE_MAIN_SWITCH && MEMADE_BROWSER_PROCESS_MAIN_HOOK
/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 14 Feb 2023 15:43:10 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif  ///__314894CF_D5D4_4D34_9558_18AECA81498F__