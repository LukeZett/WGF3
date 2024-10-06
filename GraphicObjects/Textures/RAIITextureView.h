#pragma once
#include <webgpu/webgpu.h>
#include "Device.h"

namespace WGF
{

	class RAIITextureView
	{
		WGPUTextureView m_texView = nullptr;

	public:
		RAIITextureView() {}

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

		RAIITextureView(const RAIITextureView&) = delete;

		RAIITextureView(RAIITextureView&& other) noexcept
		{
			m_texView = other.m_texView;
			other.m_texView = nullptr;
		}

		RAIITextureView& operator=(RAIITextureView&& other) noexcept
		{
			m_texView = other.m_texView;
			other.m_texView = nullptr;
			return *this;
		}

		~RAIITextureView()
		{
			Destroy();
		}
	};
}