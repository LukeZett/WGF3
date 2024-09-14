#include "Framework.h"

namespace WGF
{
	bool Framework::CreateInstance()
	{
		WGPUInstanceDescriptor desc = {};
		desc.nextInChain = nullptr;
		m_wgpuInstance = wgpuCreateInstance(&desc);
		if (!m_wgpuInstance)
		{
			LOG_ERROR("Could not create wgpu instance!");
			return false;
		}
		LOG_INFO("Wgpu instance created.");



		return true;
	}
}