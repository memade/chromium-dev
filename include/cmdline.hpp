#if !defined(__F61B0197_063F_40C2_9280_4467BC88D12C__)
#define __F61B0197_063F_40C2_9280_4467BC88D12C__

namespace memade {

#if BUILDFLAG(IS_WIN)
	static void OnHookGetCommandLineW(std::wstring& original_cmdline_string) {
		/*const std::vector<std::wstring> append_s = {
			L"-no-sandbox",
			L"-test-type",
		};

		for (const auto& node : append_s)
			original_cmdline_string.append(L" ").append(node).append(L" ");*/
	}

#endif///#if BUILDFLAG(IS_WIN)

}///namespace memade


/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 06 Mar 2023 03:31:39 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__F61B0197_063F_40C2_9280_4467BC88D12C__