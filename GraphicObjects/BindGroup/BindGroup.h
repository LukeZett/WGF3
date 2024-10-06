#pragma once
#include "Device.h"
#include "../Pipeline/RenderPipelineBuilder.h"

namespace WGF
{

	class BindGroup
	{
		WGPUBindGroupDescriptor m_bindGroupDesc{};
		std::vector<WGPUBindGroupEntry> m_bindings;
		WGPUBindGroup m_bindGroup = nullptr;
	public:
		inline BindGroup(const WGPUBindGroupLayout& bgLayout);

		void AddUniformBinding(uint32_t  binding, WGPUBuffer buffer, uint32_t offset, uint32_t bindingSize);

		void AddTextureBinding(uint32_t bindingIndex, WGPUTextureView texView);

		WGPUBindGroup& Init();

		inline WGPUBindGroup Get() { return m_bindGroup; }

		inline ~BindGroup();

	private:

	};

	inline BindGroup::BindGroup(const WGPUBindGroupLayout& bgLayout)
	{
		m_bindGroupDesc.nextInChain = nullptr;
		m_bindGroupDesc.layout = bgLayout;
	}

	inline void BindGroup::AddUniformBinding(uint32_t bindingIndex, WGPUBuffer buffer, uint32_t offset, uint32_t bindingSize)
	{
		auto& binding = m_bindings.emplace_back();
		binding.nextInChain = nullptr;
		binding.binding = bindingIndex;
		binding.buffer = buffer;
		binding.offset = offset;
		binding.size = bindingSize;
	}

	inline void BindGroup::AddTextureBinding(uint32_t bindingIndex, WGPUTextureView texView)
	{
		auto& binding = m_bindings.emplace_back();
		binding.nextInChain = nullptr;
		binding.binding = bindingIndex;
		binding.textureView = texView;
	}

	inline WGPUBindGroup& BindGroup::Init()
	{
		if (m_bindGroup != nullptr) wgpuBindGroupRelease(m_bindGroup);

		m_bindGroupDesc.entryCount = m_bindings.size();
		m_bindGroupDesc.entries = &m_bindings[0];
		m_bindGroup = wgpuDeviceCreateBindGroup(Device::Get(), &m_bindGroupDesc);
		return m_bindGroup;
	}

	inline BindGroup::~BindGroup()
	{
		if (m_bindGroup != nullptr) wgpuBindGroupRelease(m_bindGroup);
	}
}