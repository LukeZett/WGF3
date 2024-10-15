#pragma once
#include "PipelineBuilder.h"
#include "ComputePipeline.h"

namespace WGF
{
	class ComputePipelineBuilder : public PipelineBuilder
	{
		WGPUComputePipelineDescriptor m_pipelineDesc;
	public:
		ComputePipeline Build()
		{
			std::vector<WGPUBindGroupLayout> wgpuBGLayouts;
			for (size_t i = 0; i < m_bindGroupLayouts.size(); i++)
				wgpuBGLayouts.emplace_back(m_bindGroupLayouts[i].Init());


			if (wgpuBGLayouts.size() > 0)
			{
				m_layoutDesc.nextInChain = nullptr;
				m_layoutDesc.bindGroupLayoutCount = wgpuBGLayouts.size();
				m_layoutDesc.bindGroupLayouts = &wgpuBGLayouts[0];
				m_layout = wgpuDeviceCreatePipelineLayout(Device::Get(), &m_layoutDesc);
				m_pipelineDesc.layout = m_layout;
			}

			ComputePipeline pipeline(m_pipelineDesc, m_shaderSource);

			if (m_layout != nullptr)
			{
				pipeline.m_layout = std::move(m_layout);
				pipeline.m_bindGroupLayouts = std::move(wgpuBGLayouts);
				m_pipelineDesc.layout = pipeline.m_layout;
				m_layout = nullptr;
			}

			return pipeline;
		}
	};
}