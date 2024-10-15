#pragma once
#include <webgpu/webgpu.h>
#include "Pipeline.h"

namespace WGF
{
	class ComputePipeline : public Pipeline
	{
		friend class ComputePipelineBuilder;

		WGPUComputePipeline m_pipeline = nullptr;
	public:
		inline ComputePipeline(WGPUComputePipelineDescriptor& desc, const std::filesystem::path& shaderPath);

		ComputePipeline(const ComputePipeline&) = delete;

		ComputePipeline(ComputePipeline&& other) noexcept : Pipeline(std::move(other))
		{
			m_pipeline = other.m_pipeline;
			other.m_pipeline = nullptr;
		}

		inline ~ComputePipeline();
	};

	WGF::ComputePipeline::ComputePipeline(WGPUComputePipelineDescriptor& desc, const std::filesystem::path& shaderPath)
		: Pipeline(shaderPath) {}

	inline ComputePipeline::~ComputePipeline()
	{
		if (m_pipeline != nullptr)
		{
			wgpuComputePipelineRelease(m_pipeline);
		}
	}
}