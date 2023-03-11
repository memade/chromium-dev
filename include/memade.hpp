#if !defined(__0AA9550E_7EC1_4E2D_884E_D11F1CD8FD72__)
#define __0AA9550E_7EC1_4E2D_884E_D11F1CD8FD72__

#include "include.hpp"

#include "peload.hpp"
#include "windows.hpp"
#include "chromium.hpp"
#include "fingerprint.hpp"



//#include "third_party/blink/renderer/core/frame/navigator_device_memory.h"



//C:\google\chromium\src\third_party\blink\public\common\device_memory
//class IDetour {
//public:
//	float Mine_deviceMemory() {
//		return (this->*Real_deviceMemory)();
//	}
//	static float (IDetour::* Real_deviceMemory)();
//};
//float (IDetour::* IDetour::Real_deviceMemory)() = (float (IDetour::*)()) & blink::ApproximatedDeviceMemory::GetApproximatedDeviceMemory;

namespace memade {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using tf_plugin_api_object_init = void(__stdcall*)(void**, unsigned long&);
	using tf_plugin_api_object_uninit = void(__stdcall*)(void);
	using tf_plugin_common_api = bool(__stdcall*)(void**, unsigned long&, const void*);

	using tf_component_api_object_init = void(__stdcall*)(void**, unsigned long&);
	using tf_component_api_object_uninit = void(__stdcall*)(void);
	using tf_component_common_api = bool(__stdcall*)(void**, unsigned long&, const void*);
	static const char gc_plugin_function_sign_init[] = "api_object_init";
	static const char gc_plugin_function_sign_uninit[] = "api_object_uninit";
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Component {
	public:
		inline Component(const std::string&);
		inline ~Component();
		inline void Release() const;
		inline const std::uintptr_t& Identify() const;
		inline const bool& Ready() const;
		inline tf_component_common_api Search(const std::string&) const;
	private:
		inline void Init();
		inline void UnInit();
	private:
		bool m_ready = false;
		std::uintptr_t m_identify = 0;
		std::string m_pepathname;
		std::map<std::string, tf_component_common_api> m_api_s;
		PE::HMEMORYMODULE m_module_handle = nullptr;
		tf_component_api_object_init m_api_object_init = nullptr;
		tf_component_api_object_uninit m_api_object_uninit = nullptr;
	};
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		std::string m_pepathname;
		std::map<std::string, tf_plugin_common_api> m_api_s;
		PE::HMEMORYMODULE m_module_handle = nullptr;
		tf_plugin_api_object_init m_api_object_init = nullptr;
		tf_plugin_api_object_uninit m_api_object_uninit = nullptr;
	};
	inline Plugin::Plugin(const std::string& plugin_pepathname) :
		m_pepathname(plugin_pepathname) {
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
			if (!Win::AccessA(m_pepathname))
				break;
			auto pe_buffer = Win::Read(m_pepathname);
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
			if (!m_api_object_init || !m_api_object_uninit)
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
						m_api_s.emplace(node, fcomm);
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
			auto find = m_api_s.find(function_name);
			if (find == m_api_s.end())
				break;
			found = find->second;
		} while (0);
		return found;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline Component::Component(const std::string& pepathname) :
		m_pepathname(pepathname) {
		Init();
	}
	inline Component::~Component() {
		UnInit();
	}
	inline void Component::Release() const {
		delete this;
	}
	inline const std::uintptr_t& Component::Identify() const {
		return m_identify;
	}
	inline const bool& Component::Ready() const {
		return m_ready;
	}
	inline void Component::Init() {
		do {
			if (!Win::AccessA(m_pepathname))
				break;
			auto pebuffer = Win::Read(m_pepathname);
			if (pebuffer.empty())
				break;
			m_module_handle = PE::MemoryLoadLibrary(pebuffer.data(), pebuffer.size());
			if (!m_module_handle)
				break;
			m_identify = reinterpret_cast<decltype(m_identify)>(m_module_handle);
			m_api_object_init = reinterpret_cast<decltype(m_api_object_init)>(PE::MemoryGetProcAddress(m_module_handle, gc_plugin_function_sign_init));
			m_api_object_uninit = reinterpret_cast<decltype(m_api_object_uninit)>(PE::MemoryGetProcAddress(m_module_handle, gc_plugin_function_sign_uninit));
			if (!m_api_object_init || !m_api_object_uninit)
				break;
			void* route = nullptr;
			unsigned long route_size = 0;
			m_api_object_init(&route, route_size);
			if (route && route_size > 0) {
				std::vector<std::string> funs;
				Win::SplitStringA(std::string((char*)route, route_size), ',', funs);
				for (const auto& node : funs) {
					tf_component_common_api fcomm = \
						reinterpret_cast<tf_component_common_api>(PE::MemoryGetProcAddress(m_module_handle, node.data()));
					if (fcomm)
						m_api_s.emplace(node, fcomm);
				}
			}
			MEMADE_ROUTE_FREE(route);
			m_ready = true;
		} while (0);
	}
	inline void Component::UnInit() {
		do {

		} while (0);
	}
	inline tf_component_common_api
		Component::Search(const std::string& function_name) const {
		tf_plugin_common_api found = nullptr;
		do {
			if (function_name.empty())
				break;
			auto find = m_api_s.find(function_name);
			if (find == m_api_s.end())
				break;
			found = find->second;
		} while (0);
		return found;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Projects final : public IFingerprint {
		std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
	public:
		inline Projects();
		inline	~Projects();
		inline void Release() const;
	public:
		inline void Init();
		inline tf_plugin_common_api FindPluginApi(const std::string&) const;
		inline void FingerprintInit() override;
		inline void FingerprintUnInit() override;
		inline bool ComponentCall(const std::string& function_name, void**, unsigned long&, const void*) override;
	private:
		inline void UnInit();
	private:
		std::map<std::uintptr_t, Plugin*> m_plugin_s;
		std::map<std::uintptr_t, Component*> m_component_s;
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
		std::string log;
		log.append("Memade process({}").append(std::to_string(GetCurrentProcessId())).append(")").append("\n");
		Win::WriteAddto("D:\\viewlog\\log.log", log);
		do {//!@ init compones
			std::string compones_path = Win::GetModulePathA() + R"(\components\)";
#if _DEBUG
			compones_path = R"(C:\google\chromium-dev\components\)";
#endif
			Win::tfEnumFolderNode enum_files, enum_folders;
			Win::EnumFoldersAndFilesA(compones_path, enum_folders, enum_files, "*.dll");
			if (enum_files.empty())
				break;
			for (const auto& pe_node : enum_files) {
				const std::string current_pathanme = compones_path + pe_node.second;
				do {
					Component* comp = new Component(current_pathanme);
					if (!comp->Ready()) {
						comp->Release();
						break;
					}
					m_component_s.emplace(comp->Identify(), comp);
				} while (0);
			}

		} while (0);

		/*m_CurrentParentProcessId = \
			base::GetParentProcessId(base::GetCurrentProcessHandle());*/
		do {//!@ init plugin modules
			std::string plugins_path = Win::GetModulePathA() + R"(\plugins\)";
#if _DEBUG
			plugins_path = R"(D:\github\Windows\projects\browser_plugins\bin\x64\Release\)";
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

		FingerprintInit();
	}
	inline void Projects::UnInit() {
		FingerprintUnInit();
		for (auto& node : m_plugin_s)
			node.second->Release();
		for (auto& node : m_component_s)
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

#if 0
	inline void Projects::HookInit() {
#if 0
#if 1
#if 0
		float ApproximatedDeviceMemory::GetApproximatedDeviceMemory() {
			return approximated_device_memory_gb_;
		}
#endif

#if 0//!@ successful case
		unsigned long route_size = 0;
		void* local_MessageBoxA = (void*)MessageBoxA;
		static auto remote_MessageBoxA = [](_In_opt_ HWND hWnd,
			_In_opt_ LPCSTR lpText,
			_In_opt_ LPCSTR lpCaption,
			_In_ UINT uType)->int {

				return 0;
		};
		ComponentCall("Attach", &local_MessageBoxA, route_size, (const void*)remote_MessageBoxA);
		::MessageBoxA(NULL, NULL, NULL, NULL);


		unsigned long route_size = 0;
		void* local_GetApproximatedDeviceMemory = (void*)blink::ApproximatedDeviceMemory::GetApproximatedDeviceMemory;
		static auto remote_GetApproximatedDeviceMemory =
			[](void)->float {
			::MessageBoxW(NULL, NULL, NULL, NULL);
			return 22.0;
		};
		ComponentCall("Attach", &local_GetApproximatedDeviceMemory, route_size, (const void*)remote_GetApproximatedDeviceMemory);
#endif







		//float ret = blink::ApproximatedDeviceMemory::GetApproximatedDeviceMemory();
		//assert(ret);
	//	void* local_GetApproximatedDeviceMemory = (void*)&blink::ApproximatedDeviceMemory::GetApproximatedDeviceMemory;
	//	unsigned long route_size = 0;
	//ComponentCall("Attach", &local_GetApproximatedDeviceMemory, route_size, (const void*)remote_GetApproximatedDeviceMemory);
#endif
#if 0
		//!@ [X:\google\chromium\src\third_party\blink\renderer\core\frame\navigator_device_memory.h] line:12
		namespace blink {
			class CORE_EXPORT NavigatorDeviceMemory {
			public:
				float deviceMemory() const;
			};
		}  // namespace blink
#endif



		//void* local_deviceMemory = (void*)blink::NavigatorDeviceMemory::deviceMemory;
		//unsigned long route_size = 0;
		//static auto remote_deviceMemory = []()->float {
		//	return 0.0;
		//};

		//ComponentCall("Attach", &local_deviceMemory, route_size, (const void*)remote_deviceMemory);
		//::MessageBoxA(NULL, "ASDGASDGA", NULL, NULL);
		//assert(ret);
		//float (blink::NavigatorDeviceMemory::* pLoad)() = &blink::NavigatorDeviceMemory::deviceMemory;
		//float (CDetour::* pRemote)(void) = &CDetour::Mine_Target;
		//ComponentCall("Attach", &(void*&)CDetour::Real_Target, route_size, (const void*)(*(PBYTE*)&pRemote));
		//Attach(&(void*&)CDetour::Real_Target, *(PBYTE*)&pRemote);

		//::MessageBoxA(NULL, "ASDGASDGA", NULL, NULL);
#endif

#if 1//success
		unsigned long route_size = 0;
		//float (Test::* pLoad)(void) = &Test::TestPublic;
		float (CDetour:: * pRemote)(void) const = &CDetour::Mine_Target;
		//Attach(&(void*&)CDetour::Real_Target, *(PBYTE*)&pRemote);
		ComponentCall("Attach", &(void*&)CDetour::Real_Target, route_size, (const void*)(*(PBYTE*)&pRemote));
#endif

#if 0
		int SysInfo::NumberOfProcessors() {
			return win::OSInfo::GetInstance()->processors();
		}
#endif

	}
	inline void Projects::HookUnInit() {

	}
#endif


	static Projects* __gpsProjects = nullptr;






#if 0
	static auto defGetApproximatedDeviceMemoryRemote = [](
		)->float {
			//return 8.0;
			return __gpsProjects->GetApproximatedDeviceMemoryLocal();
	};
	static auto defNumberOfProcessorsRemote = [](
		)->int {
			//return 19;
			return __gpsProjects->NumberOfProcessorsLocal();
	};
#endif
	inline void Projects::FingerprintInit() {
#if 0
		unsigned long route_size = 0;
		ComponentCall("Attach", (void**)&NumberOfProcessorsLocal, route_size, (const void*)defNumberOfProcessorsRemote);
		ComponentCall("Attach", (void**)&GetApproximatedDeviceMemoryLocal, route_size, (const void*)defGetApproximatedDeviceMemoryRemote);
#endif
	}
	inline void Projects::FingerprintUnInit() {

	}
	inline bool Projects::ComponentCall(const std::string& function_name, void** route, unsigned long& route_size, const void* view) {
		bool result = false;
		std::lock_guard<std::mutex> lock{ *m_Mutex };
		do {
			if (function_name.empty())
				break;
			tf_component_common_api found = nullptr;
			for (const auto& node : m_component_s) {
				found = node.second->Search(function_name);
				if (found)
					break;
			}
			if (!found)
				break;
			result = found(route, route_size, view);
		} while (0);
		return result;
	}







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
