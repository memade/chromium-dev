#if !defined(__49100E7E_583A_4BDD_A27F_F00F40707DDE__)
#define __49100E7E_583A_4BDD_A27F_F00F40707DDE__

#define MEMADE_ROUTE_FREE(p)\
do {\
if (!p)\
break;\
if (FALSE == ::HeapFree(::GetProcessHeap(), 0, p)) \
break;\
p = nullptr;\
} while (0);\

namespace memade {

	class Win final {
	public:
		using tfEnumFolderNode = std::map<std::string, std::string>;
	public:
		static bool AccessA(const std::string& PathOrPathname) {
			bool result = false;
			do {
				if (PathOrPathname.empty())
					break;
				result = ::_access(PathOrPathname.c_str(), 0) == 0;
			} while (0);
			return result;
		}
		static std::string JsonToString(const rapidjson::Value& valObj) {
			std::string result;
			rapidjson::StringBuffer jbuffer;
			rapidjson::Writer<rapidjson::StringBuffer> jwriter(jbuffer);
			if (valObj.Accept(jwriter))
				result = std::string(jbuffer.GetString(), jbuffer.GetLength());
			return result;
		}
		static std::string GetModulePathA(const HINSTANCE& hModule = nullptr) {
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
		static std::string GetModulePathnameA(const HINSTANCE& hModule = nullptr) {
			std::string result;
			do {
				char Filename[MAX_PATH] = { 0 };
				if (::GetModuleFileNameA(hModule, Filename, MAX_PATH) <= 0)
					break;
				result = Filename;
			} while (0);
			return result;
		}
		static std::string EnumCorrectPathA(const std::string& in, const char& replace_at = '\\', const bool& isFolder = true) {
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
		static void EnumFoldersAndFilesA(
			const std::string& Path,
			tfEnumFolderNode& Folders,
			tfEnumFolderNode& Files,
			const char* FileFilter = "*.*",
			bool bSleepDirect = false,
			const std::function<void(const std::string&, const std::string&, const _finddata_t&)>& enumcb = nullptr) {
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
									Folders.emplace(std::make_pair(EnumCorrectPathA(the), FindData.name));
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
								Files.emplace(std::make_pair(EnumCorrectPathA(the, '\\', false), FindData.name));
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
								Folders.emplace(std::make_pair(EnumCorrectPathA(the), FindData.name));
								if (enumcb) {
									enumcb(the, FindData.name, FindData);
								}
								break;
							}
							auto the = path_pos + "\\" + FindData.name;
							the.erase(the.find(Path), Path.size());
							Files.emplace(std::make_pair(EnumCorrectPathA(the, '\\', false), FindData.name));
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

		static void SplitStringA(const std::string& in, const char& delimiter, std::vector<std::string>& out) {
			out.clear();
			do {
				if (in.empty())
					break;
				std::string tempContent{ in };
				size_t found = 0;
				while ((found = tempContent.find(delimiter)) != std::string::npos) {
					std::string strSession;
					strSession.insert(strSession.length(), tempContent.data(), found);
					tempContent.erase(0, found + 1);
					out.push_back(strSession);
				}
				if (tempContent.empty())
					break;
				out.push_back(tempContent);
			} while (0);
		}
		static std::string Read(const std::string& FilePathname, const int& OpenMode = std::ios::out | std::ios::in | std::ios::binary/*0x20 | 0x40 | 0x80*//*std::ios::_Nocreate | std::ios::_Noreplace | std::ios::binary*/) {
			std::string result;
			std::fstream of(FilePathname, OpenMode);
			/// Buffering was added to avoid crashes on the X86 platform
			char* buffer = nullptr;
			do {
				if (!of.is_open())
					break;
				of.seekg(0, of.end);
				size_t size = static_cast<size_t>(of.tellg());
				if (size <= 0)
					break;
				buffer = reinterpret_cast<char*>(::malloc(size));
				if (!buffer)
					break;
				of.seekg(0, of.beg);
				of.read(buffer, size);
				result.append(buffer, size);
			} while (0);
			if (of.is_open())
				of.close();
			if (buffer) {
				::free(buffer);
				buffer = nullptr;
			}
			return result;
		}

		static std::string PathFixedA(const std::string& PathOrPathname) {
			std::string result{ PathOrPathname };
			if (result.empty())
				return result;
			for (auto it = result.begin(); it != result.end(); ++it) {
				if (*it == '/')
					*it = '\\';
			}
			do {
				auto found = result.find("\\\\");
				if (found == std::string::npos)
					break;
				result.replace(found, ::strlen("\\\\"), "\\");
			} while (1);
			return result;
		}
	};


}///namespace memade

/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 01 Mar 2023 22:25:09 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__49100E7E_583A_4BDD_A27F_F00F40707DDE__