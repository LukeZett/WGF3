#pragma once
#include "Device.h"
#include <vector>
#include <filesystem>

inline void setDefault(WGPUBindGroupLayoutEntry& bindingLayout);

namespace WGF
{
	class BindGroupLayout
	{
		friend class RenderPipelineBuilder;
		friend class ComputePipelineBuilder;

		WGPUBindGroupLayoutDescriptor m_desc = {};
		std::vector<WGPUBindGroupLayoutEntry> m_entries;


		WGPUBindGroupLayout Init()
		{
			m_desc.entryCount = m_entries.size();
			m_desc.entries = &m_entries[0];
			return wgpuDeviceCreateBindGroupLayout(Device::Get(), &m_desc);
		}

	public:

		BindGroupLayout& AddUniformBinding(uint32_t binding, WGPUShaderStageFlags visibility, size_t uniformSize)
		{
			auto& entry = m_entries.emplace_back();
			setDefault(entry);
			entry.binding = binding;
			entry.visibility = visibility;
			entry.buffer.type = WGPUBufferBindingType_Uniform;
			entry.buffer.minBindingSize = uniformSize;
			return *this;
		}

		BindGroupLayout& AddStorageBinding(uint32_t binding, WGPUShaderStageFlags visibility)
		{
			auto& entry = m_entries.emplace_back();
			setDefault(entry);
			entry.binding = binding;
			entry.visibility = visibility;
			entry.buffer.type = WGPUBufferBindingType_Storage;
			return *this;
		}

		BindGroupLayout& AddTextureBinding(uint32_t binding, WGPUShaderStageFlags visibility, WGPUTextureViewDimension dimension, WGPUTextureSampleType sampleType)
		{
			auto& entry = m_entries.emplace_back();
			setDefault(entry);
			entry.binding = binding;
			entry.visibility = visibility;
			entry.texture.sampleType = sampleType;
			entry.texture.viewDimension = dimension;
			return *this;
		}

		BindGroupLayout& AddTextureStorage(uint32_t binding, WGPUShaderStageFlags visibility,
											WGPUStorageTextureAccess access, WGPUTextureFormat format, WGPUTextureViewDimension dimension)
		{
			auto& entry = m_entries.emplace_back();
			setDefault(entry);
			entry.binding = binding;
			entry.visibility = visibility;
			entry.storageTexture.access = access;
			entry.storageTexture.format = format;
			entry.storageTexture.viewDimension = dimension;
			return *this;
		}
	};

	class PipelineBuilder
	{
	protected:
		std::filesystem::path m_shaderSource = "";
		WGPUPipelineLayoutDescriptor m_layoutDesc = {};
		WGPUPipelineLayout m_layout = nullptr;
		std::vector<BindGroupLayout> m_bindGroupLayouts = {};
	public:
		PipelineBuilder() = default;

		inline PipelineBuilder& SetShaderPath(const std::filesystem::path& source)
		{
			m_shaderSource = source;
			return *this;
		}

		inline BindGroupLayout& AddBindGroup()
		{
			return m_bindGroupLayouts.emplace_back();
		}

		~PipelineBuilder()
		{
			if (m_layout != nullptr)
			{
				wgpuPipelineLayoutRelease(m_layout);
			}
		}
	};


}

inline void setDefault(WGPUBindGroupLayoutEntry& bindingLayout) {
	bindingLayout.buffer.nextInChain = nullptr;
	bindingLayout.buffer.type = WGPUBufferBindingType_Undefined;
	bindingLayout.buffer.hasDynamicOffset = false;

	bindingLayout.sampler.nextInChain = nullptr;
	bindingLayout.sampler.type = WGPUSamplerBindingType_Undefined;

	bindingLayout.storageTexture.nextInChain = nullptr;
	bindingLayout.storageTexture.access = WGPUStorageTextureAccess_Undefined;
	bindingLayout.storageTexture.format = WGPUTextureFormat_Undefined;
	bindingLayout.storageTexture.viewDimension = WGPUTextureViewDimension_Undefined;

	bindingLayout.texture.nextInChain = nullptr;
	bindingLayout.texture.multisampled = false;
	bindingLayout.texture.sampleType = WGPUTextureSampleType_Undefined;
	bindingLayout.texture.viewDimension = WGPUTextureViewDimension_Undefined;
}

