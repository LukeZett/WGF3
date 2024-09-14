#include "RenderPipeline.h"
#include "Framework.h"

void WGF::RenderPipeline::Init(WGPURenderPipelineDescriptor& pipelineDesc, WGPUFragmentState& fragState)
{
	m_passDesc.nextInChain = nullptr;
	m_passDesc.depthStencilAttachment = nullptr;
	m_passDesc.timestampWrites = nullptr;
	m_passColorAttachment.loadOp = WGPULoadOp_Clear;
	m_passColorAttachment.storeOp = WGPUStoreOp_Store;
	m_passColorAttachment.clearValue = WGPUColor{ 0.0, 0.0, 0.0, 1.0 };
#ifndef WEBGPU_BACKEND_WGPU
	m_passColorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif // NOT WEBGPU_BACKEND_WGPU
	m_passDesc.colorAttachmentCount = 1;
	m_passDesc.depthStencilAttachment = nullptr;
	m_passDesc.colorAttachments = &m_passColorAttachment;

	m_passDepthAttachment.depthClearValue = 1.0f;
	m_passDepthAttachment.depthLoadOp = WGPULoadOp_Clear;
	m_passDepthAttachment.depthStoreOp = WGPUStoreOp_Store;
	m_passDepthAttachment.depthReadOnly = false;

	m_passDepthAttachment.stencilClearValue = 0;
	m_passDepthAttachment.stencilLoadOp = WGPULoadOp_Clear;
	m_passDepthAttachment.stencilStoreOp = WGPUStoreOp_Store;
	m_passDepthAttachment.stencilReadOnly = true;

	fragState.module = m_shaderModule.Get();
	pipelineDesc.vertex.module = m_shaderModule.Get();
	m_pipeline = wgpuDeviceCreateRenderPipeline(Device::Get(), &pipelineDesc);
}
