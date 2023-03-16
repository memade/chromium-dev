#if !defined(__837D8F62_85EC_4538_8FDF_9BC9D0D60C3C__)
#define __837D8F62_85EC_4538_8FDF_9BC9D0D60C3C__

// Use this head file requires the following
// [..\chromium\src\chrome\browser\extensions\component_loader.cc]
//Line 57 -> add [#include "../../../../../chromium-dev/include/extensions.hpp"]
//Line 456 -> add [....]
#pragma region{codebak}
#if 0
#if MEMADE_ENABLE_EXTENSION
do {
	std::set<memade::ExtensionNode> append_extensions;
	memade::Extensions::Load(append_extensions);
	if (append_extensions.empty())
		break;
	for (auto& extension : append_extensions) {
		absl::optional<base::Value::Dict> manifest_test = ParseManifest(extension.ManifestBuffer);
		if (!manifest_test)
			continue;
		manifest_test->Set(manifest_keys::kName, extension.Name);
		manifest_test->Set(manifest_keys::kDescription, extension.Description);
		ComponentExtensionInfo info(std::move(*manifest_test), base::FilePath(extension.PathW));
		component_extensions_.push_back(std::move(info));
		ComponentExtensionInfo& added_info = component_extensions_.back();
		if (!extension_system_->is_ready())
			continue;
		Load(added_info);
	}
} while (0);
#endif///#if MEMADE_ENABLE_EXTENSION
#endif
#pragma endregion{end codebak}

#include "include.hpp"
#include "windows.hpp"
#define MEMADE_ENABLE_EXTENSION 1

namespace memade {
	class ExtensionNode final {
	public:
		std::string PathA;
		std::wstring PathW;
		std::string Name;
		std::string Description;
		std::string ManifestBuffer;
		std::string ManifestPathname;
		bool operator<(const ExtensionNode& obj) const {
			return PathW.compare(obj.PathW) < 0;
		}
		bool operator>(const ExtensionNode& obj) const {
			return PathW.compare(obj.PathW) > 0;
		}
		bool operator==(const ExtensionNode& obj) const {
			return PathW.compare(obj.PathW) == 0;
		}
		bool operator!=(const ExtensionNode& obj) const {
			return PathW.compare(obj.PathW) != 0;
		}
	};

	using ExtensionNodes = std::set<ExtensionNode>;

	class Extensions final {
	public:
		inline static void Load(ExtensionNodes&);
	};

	inline void Extensions::Load(ExtensionNodes& outExtensions) {
		outExtensions.clear();
		do {
			std::string system_extensions_path = Win::GetModulePathA() + "\\memade\\extensions\\";
			//!@ 当前是测试开发阶段 依然使用扩展开发目录
#if 1//_DEBUG
			system_extensions_path = R"(C:\google\chromium-dev\res\extensions\)";
#endif
			Win::tfEnumFolderNode files, folders;
			Win::EnumFoldersAndFilesA(system_extensions_path, folders, files);

			for (auto& node : folders) {
				ExtensionNode ext_node;
				ext_node.Name = node.second;
				ext_node.PathA = Win::PathFixedA(system_extensions_path + node.first);
				ext_node.PathW = Win::ASCIIToWide(ext_node.PathA);
				ext_node.ManifestPathname = Win::PathFixedA(ext_node.PathA + R"(manifest.json)");
				ext_node.ManifestBuffer = Win::Read(ext_node.ManifestPathname);
				if (ext_node.ManifestBuffer.empty())
					continue;
				outExtensions.emplace(ext_node);
			}
		} while (0);
	}
}///namespace memade


#if 0//!@ https://developer.chrome.com/docs/extensions/mv2/manifest/
Manifest file format
Published on Tuesday, September 18, 2012 • Updated on Thursday, April 26, 2018
Warning
The Chrome Web Store no longer accepts Manifest V2 extensions.Please use Manifest V3 when building new
extensions.You will find a section on upgrading in the navigation tree at the left, including the Manifest V2 support timeline.
Every extension has a JSON - formatted manifest file, named manifest.json, that provides important information.
#Field summary
The following code shows the supported manifest fields for Extensions, with links to the page that discusses each field.

{
	// Required
	"manifest_version": 2,
		"name" : "My Extension",
		"version" : "versionString",

		// Recommended
		"default_locale" : "en",
		"description" : "A plain text description",
		"icons" : {...},

		// Pick one (or none)
		"browser_action" : {...},
		"page_action" : {...},

		// Optional
		"action" : ...,
		"author" : ...,
		"automation" : ...,
		"background" : {
		// Recommended
		"persistent": false,
			// Optional
			"service_worker" : ...
	},
		"chrome_settings_overrides": {...},
			"chrome_url_overrides" : {...},
			"commands" : {...},
			"content_capabilities" : ...,
			"content_scripts" : [{...}] ,
			"content_security_policy" : "policyString",
			"converted_from_user_script" : ...,
			"cross_origin_embedder_policy" : {"value": "require-corp"},
			"cross_origin_opener_policy" : {"value": "same-origin"},
			"current_locale" : ...,
			"declarative_net_request" : ...,
			"devtools_page" : "devtools.html",
			"differential_fingerprint" : ...,
			"event_rules" : [{...}] ,
			"externally_connectable" : {
			"matches": ["*://*.example.com/*"]
		},
			"file_browser_handlers" : [...] ,
				"file_system_provider_capabilities" : {
				"configurable": true,
					"multiple_mounts" : true,
					"source" : "network"
			},
				"homepage_url" : "http://path/to/homepage",
					"host_permissions" : ...,
					"import" : [{"id": "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}] ,
					"incognito" : "spanning, split, or not_allowed",
					"input_components" : ...,
					"key" : "publicKey",
					"minimum_chrome_version" : "versionString",
					"nacl_modules" : [...] ,
					"natively_connectable" : ...,
					"oauth2" : ...,
					"offline_enabled" : true,
					"omnibox" : {
					"keyword": "aString"
				},
					"optional_permissions" : ["tabs"] ,
						"options_page" : "options.html",
						"options_ui" : {
						"chrome_style": true,
							"page" : "options.html"
					},
						"permissions" : ["tabs"] ,
							"platforms" : ...,
							"replacement_web_app" : ...,
							"requirements" : {...},
							"sandbox" : [...] ,
							"short_name" : "Short Name",
							"storage" : {
							"managed_schema": "schema.json"
						},
							"system_indicator" : ...,
								"tts_engine" : {...},
								"update_url" : "http://path/to/updateInfo.xml",
								"version_name" : ...,
								"web_accessible_resources" : [...]
}
Updated on Thursday, April 26, 2018 • Improve article
#endif
/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 08 Mar 2023 00:52:43 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__837D8F62_85EC_4538_8FDF_9BC9D0D60C3C__
