#pragma once
#include "webgpu/webgpu.h"
#include "Shader/Shader.h"

namespace WGF
{
	class Pipeline
	{
		friend class RenderPipelineBuilder;

	protected:
		WGPUPipelineLayout m_layout = nullptr;
		std::vector<WGPUBindGroupLayout> m_bindGroupLayouts;
		Shader m_shaderModule;

	public:
		Pipeline() {}

		Pipeline(const std::filesystem::path& shaderSource) : m_shaderModule(shaderSource) {}

		inline const WGPUBindGroupLayout& GetBindGroupLayout(uint32_t index) const
		{
			return m_bindGroupLayouts[index];
		}

		Pipeline(const Pipeline&) = delete;

		Pipeline(Pipeline&& other) noexcept
		{
			m_layout = other.m_layout;
			other.m_layout = nullptr;
			m_bindGroupLayouts = std::move(other.m_bindGroupLayouts);
			m_shaderModule = std::move(other.m_shaderModule);
		}

		virtual ~Pipeline()
		{
			if (m_layout == nullptr) 
				return;
			
			wgpuPipelineLayoutRelease(m_layout);
			for (auto& bgLayout : m_bindGroupLayouts)
				wgpuBindGroupLayoutRelease(bgLayout);
		}
	};
}