#pragma once
#include "RenderPass.h"
#include <webgpu/webgpu.h>
#include <optional>
#include <array>
#include "Device.h"


namespace WGF
{
	template<size_t colorAttachments>
	class RenderPassFactory
	{
		WGPURenderPassDescriptor m_desc{};
		std::array<WGPURenderPassColorAttachment, colorAttachments> m_colorAttachments;
		WGPURenderPassDepthStencilAttachment m_depthAttachment{};
	public:
		inline RenderPassFactory();

		inline RenderPass BeginPass(std::array<WGPUTextureView, colorAttachments>& target, std::optional<WGPUTextureView> depthTarget);

		inline void EndPass(RenderPass& pass);

		~RenderPassFactory() = default;
	};

	template<size_t colorAttachments>
	inline RenderPassFactory<colorAttachments>::RenderPassFactory()
	{
		m_desc.label = "Render pass";
		m_desc.nextInChain = nullptr;
		m_desc.depthStencilAttachment = nullptr;
		m_desc.timestampWrites = nullptr;
		m_desc.colorAttachmentCount = colorAttachments;
		m_desc.colorAttachments = &m_colorAttachments[0];
		m_desc.depthStencilAttachment = nullptr;

		m_depthAttachment.depthClearValue = 1.0f;
		m_depthAttachment.depthLoadOp = WGPULoadOp_Clear;
		m_depthAttachment.depthStoreOp = WGPUStoreOp_Store;
		m_depthAttachment.depthReadOnly = false;

		m_depthAttachment.stencilClearValue = 0;
		m_depthAttachment.stencilLoadOp = WGPULoadOp_Clear;
		m_depthAttachment.stencilStoreOp = WGPUStoreOp_Store;
		m_depthAttachment.stencilReadOnly = true;

		for (size_t i = 0; i < colorAttachments; i++)
		{
			m_colorAttachments[i].nextInChain = nullptr;
			m_colorAttachments[i].loadOp = WGPULoadOp_Clear;
			m_colorAttachments[i].storeOp = WGPUStoreOp_Store;
			m_colorAttachments[i].clearValue = WGPUColor{ 0.0, 0.0, 0.0, 1.0 };
#ifndef WEBGPU_BACKEND_WGPU
			m_colorAttachments[i].depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif // NOT WEBGPU_BACKEND_WGPU
		}
	}

	template<size_t colorAttachments>
	inline RenderPass RenderPassFactory<colorAttachments>::BeginPass(std::array<WGPUTextureView, colorAttachments>& target, std::optional<WGPUTextureView> depthTarget)
	{
		
		for (size_t i = 0; i < colorAttachments; i++)
		{
			m_colorAttachments[i].view = target[i];
			m_colorAttachments[i].resolveTarget = nullptr;
		}
		m_desc.colorAttachments = &m_colorAttachments[0];
		
		if (depthTarget.has_value())
		{
			m_depthAttachment.view = depthTarget.value();
			m_depthAttachment.depthClearValue = 1.0f;
			m_depthAttachment.depthLoadOp = WGPULoadOp_Clear;
			m_desc.depthStencilAttachment = &m_depthAttachment;
		}
		else {
			m_desc.depthStencilAttachment = nullptr;
		}

		Device::CreateEncoder();
		return RenderPass(wgpuCommandEncoderBeginRenderPass(Device::GetEncoder(), &m_desc));
	}

	template<size_t colorAttachments>
	inline void RenderPassFactory<colorAttachments>::EndPass(RenderPass& pass)
	{
		wgpuRenderPassEncoderEnd(pass.m_pass);
		wgpuRenderPassEncoderRelease(pass.m_pass);
		pass.m_pass = nullptr;

		WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
		cmdBufferDescriptor.nextInChain = nullptr;
		cmdBufferDescriptor.label = "Command buffer";
		WGPUCommandBuffer command = wgpuCommandEncoderFinish(Device::GetEncoder(), &cmdBufferDescriptor);
		Device::ReleaseEncoder();
		wgpuQueueSubmit(Device::GetQueue(), 1, &command);
		wgpuCommandBufferRelease(command);
	}
}