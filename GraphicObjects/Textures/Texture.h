#pragma once
#include <webgpu/webgpu.h>
#include "Device.h"

namespace WGF
{

	class Texture
	{
		WGPUTexture m_texture = nullptr;

	public:
		Texture() {}

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

		Texture(const Texture&) = delete;

		Texture(Texture&& other) noexcept
		{
			m_texture = other.m_texture;
			other.m_texture = nullptr;
		}

		Texture& operator=(Texture&& other) noexcept
		{
			m_texture = other.m_texture;
			other.m_texture = nullptr;
			return *this;
		}
	};
}