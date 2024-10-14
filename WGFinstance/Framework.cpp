#include "Framework.h"

namespace WGF
{
	bool Framework::CreateInstance()
	{
		WGPUInstanceDescriptor desc = {};
		desc.nextInChain = nullptr;
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
		m_wgpuInstance = wgpuCreateInstance(nullptr);
#else //  WEBGPU_BACKEND_EMSCRIPTEN
		m_wgpuInstance = wgpuCreateInstance(&desc);
#endif //  WEBGPU_BACKEND_EMSCRIPTEN
		if (!m_wgpuInstance)
		{
			LOG_ERROR("Could not create wgpu instance!");
			return false;
		}
		LOG_INFO("Wgpu instance created.");



		return true;
	}
}