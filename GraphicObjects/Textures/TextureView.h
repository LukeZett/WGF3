#pragma once
#include <webgpu/webgpu.h>
#include "Device.h"

namespace WGF
{

	class TextureView
	{
		WGPUTextureView m_texView = nullptr;

	public:
		TextureView() {}

		void Init(WGPUTexture& tex, WGPUTextureViewDescriptor& desc)
		{
			Destroy();
			m_texView = wgpuTextureCreateView(tex, &desc);
		}

		void Destroy()
		{
			if (m_texView == nullptr) return;
			wgpuTextureViewRelease(m_texView);
			m_texView = nullptr;
		}

		WGPUTextureView& Get() { return m_texView; }

		TextureView(const TextureView&) = delete;

		TextureView(TextureView&& other) noexcept
		{
			m_texView = other.m_texView;
			other.m_texView = nullptr;
		}

		TextureView& operator=(TextureView&& other) noexcept
		{
			m_texView = other.m_texView;
			other.m_texView = nullptr;
			return *this;
		}
	};
}