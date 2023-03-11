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
		static std::wstring PathFixedW(const std::wstring& PathOrPathname) {
			std::wstring result{ PathOrPathname };
			if (PathOrPathname.empty())
				return result;
			for (auto it = result.begin(); it != result.end(); ++it) {
				if (*it == '/')
					*it = '\\';
			}
			do {
				auto found = result.find(L"\\\\");
				if (found == std::wstring::npos)
					break;
				result.replace(found, ::wcslen(L"\\\\"), L"\\");
			} while (1);
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
		static std::string Read(const std::string& FilePathname, const std::ios_base::openmode& OpenMode = std::ios::out | std::ios::in | std::ios::binary/*0x20 | 0x40 | 0x80*//*std::ios::_Nocreate | std::ios::_Noreplace | std::ios::binary*/) {
			std::string result;
			std::fstream of(FilePathname, OpenMode);
			/// Buffering was added to avoid crashes on the X86 platform
			char* buffer = nullptr;
			do {
				if (!of.is_open())
					break;
				of.seekg(0, of.end);
				std::streamsize size = static_cast<std::streamsize>(of.tellg());
				if (size <= 0)
					break;
				buffer = reinterpret_cast<char*>(::malloc(static_cast<size_t>(size)));
				if (!buffer)
					break;
				of.seekg(0, of.beg);
				of.read(buffer, size);
				result.append(buffer, static_cast<size_t>(size));
			} while (0);
			if (of.is_open())
				of.close();
			if (buffer) {
				::free(buffer);
				buffer = nullptr;
			}
			return result;
		}
		static bool Write(const std::string& FilePathname, const std::string& WriteData, const std::ios_base::openmode& OpenMode = std::ios::binary | std::ios::out | std::ios::trunc) {
			bool result = false;
			do {
				if (WriteData.empty())
					break;
				std::ofstream of(FilePathname, OpenMode);
				if (!of.is_open())
					break;
				of << WriteData;
				of.close();
				result = true;
			} while (0);
			return result;
		}
		static bool WriteAddto(const std::string& FilePathname, const std::string& WriteData, const std::ios_base::openmode& OpenMode = std::ios::binary | std::ios::out | std::ios::app) {
			bool result = false;
			do {
				if (WriteData.empty())
					break;
				std::ofstream of(FilePathname, OpenMode);
				if (!of.is_open())
					break;
				of << WriteData;
				of.close();
				result = true;
			} while (0);
			return result;
		}

		static std::wstring ASCIIToWide(const std::string& ascii) {
			return std::wstring(ascii.begin(), ascii.end());
		}
		static std::string WideToASCII(const std::wstring& wide) {
			return std::string(wide.begin(), wide.end());
		}
		static std::u16string ASCIIToUTF16(const std::string& ascii) {
			return std::u16string(ascii.begin(), ascii.end());
		}
		static std::string UTF16ToASCII(const std::wstring& utf16) {
			return std::string(utf16.begin(), utf16.end());
		}
#if 0
	public:
		// SrcSize is in wide characters, not in bytes.
		static byte* WideToRaw(const wchar_t* Src, byte* Dest, size_t SrcSize) {
			for (size_t I = 0; I < SrcSize; I++, Src++)
			{
				Dest[I * 2] = (byte)*Src;
				Dest[I * 2 + 1] = (byte)(*Src >> 8);
				if (*Src == 0)
					break;
			}
			return Dest;
		}
		static std::string WideToRaw(const std::wstring& input) {
			std::string result;
			do {
				if (input.empty())
					break;
				result.resize(input.size() * sizeof(wchar_t), 0x00);
				WideToRaw(input.data(), (byte*)&result[0], result.size());
			} while (0);
			return result;
		}
		static wchar_t* RawToWide(const byte* Src, wchar_t* Dest, size_t DestSize) {
			for (size_t I = 0; I < DestSize; I++)
				if ((Dest[I] = Src[I * 2] + (Src[I * 2 + 1] << 8)) == 0)
					break;
			return Dest;
		}
		static std::wstring RawToWide(const std::string& input) {
			std::wstring result;
			do {
				if (input.empty())
					break;
				result.resize(input.size(), 0x00);
				auto* r = RawToWide((const byte*)input.data(), (wchar_t*)&result[0], result.size());


				assert(r);
			} while (0);
			return result;
		}
		static void WideToUtf(const wchar_t* Src, char* Dest, size_t DestSize) {
			long dsize = (long)DestSize;
			dsize--;
			while (*Src != 0 && --dsize >= 0)
			{
				unsigned int c = *(Src++);
				if (c < 0x80)
					*(Dest++) = c;
				else
					if (c < 0x800 && --dsize >= 0)
					{
						*(Dest++) = (0xc0 | (c >> 6));
						*(Dest++) = (0x80 | (c & 0x3f));
					}
					else
					{
						if (c >= 0xd800 && c <= 0xdbff && *Src >= 0xdc00 && *Src <= 0xdfff) // Surrogate pair.
						{
							c = ((c - 0xd800) << 10) + (*Src - 0xdc00) + 0x10000;
							Src++;
						}
						if (c < 0x10000 && (dsize -= 2) >= 0)
						{
							*(Dest++) = (0xe0 | (c >> 12));
							*(Dest++) = (0x80 | ((c >> 6) & 0x3f));
							*(Dest++) = (0x80 | (c & 0x3f));
						}
						else
							if (c < 0x200000 && (dsize -= 3) >= 0)
							{
								*(Dest++) = (0xf0 | (c >> 18));
								*(Dest++) = (0x80 | ((c >> 12) & 0x3f));
								*(Dest++) = (0x80 | ((c >> 6) & 0x3f));
								*(Dest++) = (0x80 | (c & 0x3f));
							}
					}
			}
			*Dest = 0;
		}
		static size_t WideToUtfSize(const wchar_t* Src) {
			size_t Size = 0;
			for (; *Src != 0; Src++)
				if (*Src < 0x80)
					Size++;
				else
					if (*Src < 0x800)
						Size += 2;
					else
						if ((unsigned int)*Src < 0x10000) //(uint) to avoid Clang/win "always true" warning for 16-bit wchar_t.
						{
							if (Src[0] >= 0xd800 && Src[0] <= 0xdbff && Src[1] >= 0xdc00 && Src[1] <= 0xdfff)
							{
								Size += 4; // 4 output bytes for Unicode surrogate pair.
								Src++;
							}
							else
								Size += 3;
						}
						else
							if ((unsigned int)*Src < 0x200000) //(uint) to avoid Clang/win "always true" warning for 16-bit wchar_t.
								Size += 4;
			return Size + 1; // Include terminating zero.
		}
		static bool UtfToWide(const char* Src, wchar_t* Dest, size_t DestSize) {
			bool Success = true;
			long dsize = (long)DestSize;
			dsize--;
			while (*Src != 0)
			{
				unsigned int c = byte(*(Src++)), d;
				if (c < 0x80)
					d = c;
				else
					if ((c >> 5) == 6)
					{
						if ((*Src & 0xc0) != 0x80)
						{
							Success = false;
							break;
						}
						d = ((c & 0x1f) << 6) | (*Src & 0x3f);
						Src++;
					}
					else
						if ((c >> 4) == 14)
						{
							if ((Src[0] & 0xc0) != 0x80 || (Src[1] & 0xc0) != 0x80)
							{
								Success = false;
								break;
							}
							d = ((c & 0xf) << 12) | ((Src[0] & 0x3f) << 6) | (Src[1] & 0x3f);
							Src += 2;
						}
						else
							if ((c >> 3) == 30)
							{
								if ((Src[0] & 0xc0) != 0x80 || (Src[1] & 0xc0) != 0x80 || (Src[2] & 0xc0) != 0x80)
								{
									Success = false;
									break;
								}
								d = ((c & 7) << 18) | ((Src[0] & 0x3f) << 12) | ((Src[1] & 0x3f) << 6) | (Src[2] & 0x3f);
								Src += 3;
							}
							else
							{
								Success = false;
								break;
							}
				if (--dsize < 0)
					break;
				if (d > 0xffff)
				{
					if (--dsize < 0)
						break;
					if (d > 0x10ffff) // UTF-8 must end at 0x10ffff according to RFC 3629.
					{
						Success = false;
						continue;
					}
					if (sizeof(*Dest) == 2) // Use the surrogate pair.
					{
						*(Dest++) = ((d - 0x10000) >> 10) + 0xd800;
						*(Dest++) = (d & 0x3ff) + 0xdc00;
					}
					else
						*(Dest++) = d;
				}
				else
					*(Dest++) = d;
			}
			*Dest = 0;
			return Success;
		}
		// For zero terminated strings.
		static bool IsTextUtf8(const byte* Src) {
			return IsTextUtf8(Src, strlen((const char*)Src));
		}
		// Source data can be both with and without UTF-8 BOM.
		static bool IsTextUtf8(const byte* Src, size_t SrcSize) {
			while (SrcSize-- > 0)
			{
				byte C = *(Src++);
				int HighOne = 0; // Number of leftmost '1' bits.
				for (byte Mask = 0x80; Mask != 0 && (C & Mask) != 0; Mask >>= 1)
					HighOne++;
				if (HighOne == 1 || HighOne > 6)
					return false;
				while (--HighOne > 0)
					if (SrcSize-- <= 0 || (*(Src++) & 0xc0) != 0x80)
						return false;
			}
			return true;
		}
#endif
	};


}///namespace memade

/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 01 Mar 2023 22:25:09 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__49100E7E_583A_4BDD_A27F_F00F40707DDE__