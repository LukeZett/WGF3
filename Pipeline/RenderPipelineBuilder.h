#pragma once
#include <webgpu/webgpu.h>
#include "RenderPipeline.h"
#include "Framework.h"
#include "Buffer/BufferLayout.h"

void setDefault(WGPUBindGroupLayoutEntry& bindingLayout);
void setDefault(WGPUStencilFaceState& stencilFaceState);
void setDefault(WGPUDepthStencilState& depthStencilState);

namespace WGF
{
	class BindGroupLayout
	{
		friend class RenderPipelineBuilder;

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


	};

	class RenderPipelineBuilder
	{
		WGPURenderPipeline m_pipeline = nullptr;
		WGPURenderPipelineDescriptor m_pipelineDesc = {};
		WGPUFragmentState m_fragmentState{};
		WGPUBlendState m_blendState{};
		WGPUColorTargetState m_colorTarget{};
		WGPUDepthStencilState m_depthStencil{};
		std::filesystem::path m_shaderSource = "";
		
		std::vector<BufferLayout> m_bufferLayouts;
		std::vector<uint16_t> m_bufferLayoutOffsets;
		
		WGPUPipelineLayoutDescriptor m_layoutDesc = {};
		WGPUPipelineLayout m_layout = nullptr;
		std::vector<BindGroupLayout> m_bindGroupLayouts = {};

	public:
		inline RenderPipelineBuilder();

		inline RenderPipeline Build();
		
		inline RenderPipelineBuilder& SetShaderPath(const std::filesystem::path& source)
		{ 
			m_shaderSource = source;
			return *this;
		}

		inline BufferLayout& AddBufferLayout(uint16_t locationOffset, bool instanced = false);

		inline BindGroupLayout& AddBindGroup()
		{
			return m_bindGroupLayouts.emplace_back();
		}

		inline RenderPipelineBuilder& SetDepthState(bool depthWrite, WGPUTextureFormat depthTexFormat,
			WGPUCompareFunction compFun = WGPUCompareFunction_Less)
		{
			m_depthStencil.nextInChain = nullptr;
			setDefault(m_depthStencil);
			m_depthStencil.depthCompare = compFun;
			m_depthStencil.depthWriteEnabled = depthWrite;
			m_depthStencil.format = depthTexFormat;

			m_depthStencil.stencilReadMask = 0;
			m_depthStencil.stencilWriteMask = 0;
			m_pipelineDesc.depthStencil = &m_depthStencil;
			return *this;
		}
	};
}

WGF::RenderPipelineBuilder::RenderPipelineBuilder()
{
	// pipeline desc
	m_pipelineDesc.nextInChain = nullptr;

	m_pipelineDesc.vertex.bufferCount = 0;
	m_pipelineDesc.vertex.buffers = nullptr;

	m_pipelineDesc.vertex.entryPoint = "vs_main";
	m_pipelineDesc.vertex.constantCount = 0;
	m_pipelineDesc.vertex.constants = nullptr;

	m_pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
	m_pipelineDesc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
	m_pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
	m_pipelineDesc.primitive.cullMode = WGPUCullMode_None;

	m_pipelineDesc.multisample.count = 1;
	m_pipelineDesc.multisample.mask = ~0u;
	m_pipelineDesc.multisample.alphaToCoverageEnabled = false;

	m_fragmentState.entryPoint = "fs_main";
	m_fragmentState.constantCount = 0;
	m_fragmentState.constants = nullptr;

	m_blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
	m_blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
	m_blendState.color.operation = WGPUBlendOperation_Add;

	m_blendState.alpha.srcFactor = WGPUBlendFactor_Zero;
	m_blendState.alpha.dstFactor = WGPUBlendFactor_One;
	m_blendState.alpha.operation = WGPUBlendOperation_Add;

	m_colorTarget.format = WGF::Framework::Window()->GetTextureFormat();
	m_colorTarget.writeMask = WGPUColorWriteMask_All; // We could write to only some of the color channels.

	// We have only one target because our render pass has only one output color
	// attachment.
	m_fragmentState.targetCount = 1;

	m_pipelineDesc.layout = nullptr;
	m_pipelineDesc.depthStencil = nullptr;
}

inline WGF::RenderPipeline WGF::RenderPipelineBuilder::Build()
{
	m_pipelineDesc.fragment = &m_fragmentState;
	m_fragmentState.targets = &m_colorTarget;
	m_colorTarget.blend = &m_blendState;
	if (m_pipelineDesc.depthStencil != nullptr) m_pipelineDesc.depthStencil = &m_depthStencil;

	std::vector<WGPUVertexBufferLayout> layouts(m_bufferLayouts.size());
	std::vector<WGPUVertexAttribute*> layoutsAttribs(m_bufferLayouts.size());
	
	m_pipelineDesc.vertex.bufferCount = m_bufferLayouts.size();
	for (size_t i = 0; i < m_bufferLayouts.size(); i++)
	{

		m_bufferLayouts[i].GenerateVertexBufferLayout(layouts[i], layoutsAttribs[i], m_bufferLayoutOffsets[i]);
	}

	if(layouts.size() > 0) m_pipelineDesc.vertex.buffers = &layouts[0];

	std::vector<WGPUBindGroupLayout> wgpuBGLayouts;

	for (size_t i = 0; i < m_bindGroupLayouts.size(); i++)
	{
		wgpuBGLayouts.emplace_back(m_bindGroupLayouts[i].Init());
	}

	if (wgpuBGLayouts.size() > 0)
	{
		m_layoutDesc.nextInChain = nullptr;
		m_layoutDesc.bindGroupLayoutCount = wgpuBGLayouts.size();
		m_layoutDesc.bindGroupLayouts = &wgpuBGLayouts[0];
		m_layout = wgpuDeviceCreatePipelineLayout(Device::Get(), &m_layoutDesc);
		m_pipelineDesc.layout = m_layout;
	}


	RenderPipeline pipeline = m_shaderSource == "" ? 
		RenderPipeline(m_pipelineDesc, m_fragmentState) : RenderPipeline(m_shaderSource, m_pipelineDesc, m_fragmentState);

	if (m_layout != nullptr)
	{
		pipeline.m_layout = m_layout;
		pipeline.m_bindGroupLayouts = wgpuBGLayouts;
	}

	for (auto& ptr : layoutsAttribs)
	{
		delete[] ptr;
	}

	return pipeline;

}

inline WGF::BufferLayout& WGF::RenderPipelineBuilder::AddBufferLayout(uint16_t locationOffset, bool instanced)
{
	m_bufferLayoutOffsets.emplace_back(locationOffset);
	m_bufferLayouts.emplace_back(instanced);
	return m_bufferLayouts.back();
}

void setDefault(WGPUBindGroupLayoutEntry& bindingLayout) {
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

void setDefault(WGPUStencilFaceState& stencilFaceState) {
	stencilFaceState.compare = WGPUCompareFunction_Always;
	stencilFaceState.failOp = WGPUStencilOperation_Keep;
	stencilFaceState.depthFailOp = WGPUStencilOperation_Keep;
	stencilFaceState.passOp = WGPUStencilOperation_Keep;
}

void setDefault(WGPUDepthStencilState& depthStencilState) {
	depthStencilState.format = WGPUTextureFormat_Undefined;
	depthStencilState.depthWriteEnabled = false;
	depthStencilState.depthCompare = WGPUCompareFunction_Always;
	depthStencilState.stencilReadMask = 0xFFFFFFFF;
	depthStencilState.stencilWriteMask = 0xFFFFFFFF;
	depthStencilState.depthBias = 0;
	depthStencilState.depthBiasSlopeScale = 0;
	depthStencilState.depthBiasClamp = 0;
	setDefault(depthStencilState.stencilFront);
	setDefault(depthStencilState.stencilBack);
}
