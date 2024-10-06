#pragma once
#include <webgpu/webgpu.h>
#include "Device.h"

namespace WGF
{

	class RAIITexture
	{
		WGPUTexture m_texture = nullptr;

	public:
		RAIITexture() {}

		void Init(WGPUTextureDescriptor& desc)
		{
			Destroy();
			m_texture = wgpuDeviceCreateTexture(Device::Get(), &desc);
		}

		void Destroy()
		{
			if (m_texture == nullptr) return;
			wgpuTextureDestroy(m_texture);
			wgpuTextureRelease(m_texture);
			m_texture = nullptr;
		}

		WGPUTexture& Get() { return m_texture; }

		RAIITexture(const RAIITexture&) = delete;

		RAIITexture(RAIITexture&& other) noexcept
		{
			m_texture = other.m_texture;
			other.m_texture = nullptr;
		}

		RAIITexture& operator=(RAIITexture&& other) noexcept
		{
			m_texture = other.m_texture;
			other.m_texture = nullptr;
			return *this;
		}

		~RAIITexture()
		{
			Destroy();
		}
	};
}