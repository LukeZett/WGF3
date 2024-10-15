#pragma once
#include <webgpu/webgpu.h>
#include "Pipeline.h"

namespace WGF
{
	class ComputePipeline : public Pipeline
	{
		WGPUComputePipeline m_pipeline = nullptr;
	public:
		ComputePipeline(WGPUComputePipelineDescriptor& desc, const std::filesystem::path& shaderPath);
		~ComputePipeline() = default;
	private:
	};

	WGF::ComputePipeline::ComputePipeline(WGPUComputePipelineDescriptor& desc, const std::filesystem::path& shaderPath)
		: Pipeline(shaderPath)
	{

	}
}