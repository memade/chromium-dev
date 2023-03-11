#if !defined(__149F9673_4243_430B_B895_6B9254D3D73D__)
#define __149F9673_4243_430B_B895_6B9254D3D73D__

namespace memade {

	class IChromiumApi {
	protected:
		using tfNumberOfProcessors = int(__cdecl*)(void);
		using tfGetApproximatedDeviceMemory = float(__cdecl*)(void);
	public:
		tfNumberOfProcessors NumberOfProcessorsLocal = base::SysInfo::NumberOfProcessors;
		tfNumberOfProcessors NumberOfProcessorsRemote = nullptr;
		tfGetApproximatedDeviceMemory GetApproximatedDeviceMemoryLocal = blink::ApproximatedDeviceMemory::GetApproximatedDeviceMemory;
		tfGetApproximatedDeviceMemory GetApproximatedDeviceMemoryRemote = nullptr;
	};
	class IFingerprint : public IChromiumApi {
	protected:
		virtual void FingerprintInit() = 0;
		virtual void FingerprintUnInit() = 0;
		virtual bool ComponentCall(const std::string& function_name, void**, unsigned long&, const void*) = 0;
	};

}///namespace memade



/// /*_ Memade®（新生™） _**/
/// /*_ Mon, 06 Mar 2023 05:27:13 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__149F9673_4243_430B_B895_6B9254D3D73D__