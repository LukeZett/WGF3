#pragma once
#include <webgpu/webgpu.h>

namespace WGF
{
	class RenderPass
	{
		template<size_t T>
		friend class RenderPassFactory;

		WGPURenderPassEncoder m_pass{};

	public:
		WGPURenderPassEncoder Get() const { return m_pass; }

		~RenderPass() = default;

		RenderPass(WGPURenderPassEncoder pass) : m_pass(pass) {}
	private:
	};
}