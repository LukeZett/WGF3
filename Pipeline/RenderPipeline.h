#pragma once
#include <webgpu/webgpu.h>
#include "Pipeline.h"
#include "Device.h"


namespace WGF
{
	class RenderPipelineBuilder;

	class RenderPipeline : public Pipeline
	{
		friend class RenderPipelineBuilder;

		WGPURenderPassDescriptor m_passDesc = {};
		WGPURenderPassEncoder m_renderPass = nullptr;
		WGPURenderPassColorAttachment m_passColorAttachment = {};
		WGPURenderPassDepthStencilAttachment m_passDepthAttachment = {};
		WGPURenderPipeline m_pipeline = nullptr;
	
	private:
		RenderPipeline(WGPURenderPipelineDescriptor& desc, WGPUFragmentState& fragState)
		{ 
			Init(desc, fragState);
		}

		RenderPipeline(const std::filesystem::path& shaderSource, WGPURenderPipelineDescriptor& desc, WGPUFragmentState& fragState) : Pipeline(shaderSource)
		{
			Init(desc, fragState);
		}

		void Init(WGPURenderPipelineDescriptor& pipelineDesc, WGPUFragmentState& fragState);

	public:
		

		inline RenderPipeline& SetClearColor(WGPUColor color)
		{
			m_passColorAttachment.clearValue = color;
			return *this;
		}

		WGPURenderPassEncoder BeginPass(WGPUTextureView target, std::optional<WGPUTextureView> depthTarget)
		{
			Device::CreateEncoder();
			m_passColorAttachment.view = target;
			m_passColorAttachment.resolveTarget = nullptr;
			m_passDesc.colorAttachments = &m_passColorAttachment;

			if (depthTarget.has_value())
			{
				m_passDepthAttachment.view = depthTarget.value();
				m_passDepthAttachment.depthClearValue = 1.0f;
				m_passDepthAttachment.depthLoadOp = WGPULoadOp_Clear;
				m_passDesc.depthStencilAttachment = &m_passDepthAttachment;
			}
			else {
				m_passDesc.depthStencilAttachment = nullptr;
			}

			m_renderPass = wgpuCommandEncoderBeginRenderPass(Device::GetEncoder(), &m_passDesc);
			wgpuRenderPassEncoderSetPipeline(m_renderPass, m_pipeline);
			return m_renderPass;
		}

		void EndPass()
		{
			wgpuRenderPassEncoderEnd(m_renderPass);
			wgpuRenderPassEncoderRelease(m_renderPass);
			m_renderPass = nullptr;

			WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
			cmdBufferDescriptor.nextInChain = nullptr;
			cmdBufferDescriptor.label = "Command buffer";
			WGPUCommandBuffer command = wgpuCommandEncoderFinish(Device::GetEncoder(), &cmdBufferDescriptor);
			Device::ReleaseEncoder();
			wgpuQueueSubmit(Device::GetQueue(), 1, &command);
			wgpuCommandBufferRelease(command);
		}

		RenderPipeline(const RenderPipeline&) = delete;

		RenderPipeline(RenderPipeline&& other) noexcept : Pipeline(std::move(other))
		{
			m_pipeline = other.m_pipeline;
			m_passColorAttachment = other.m_passColorAttachment;
			m_passDesc = other.m_passDesc;
			m_passDesc.colorAttachments = &m_passColorAttachment;
			other.m_pipeline = nullptr;
		}

		~RenderPipeline()
		{
			if (m_pipeline != nullptr)
			{
				wgpuRenderPipelineRelease(m_pipeline);
			}
		};
	};
}