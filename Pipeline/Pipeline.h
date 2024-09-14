#pragma once
#include "webgpu/webgpu.h"
#include "Shader/Shader.h"

namespace WGF
{
	class Pipeline
	{
	public:
		Pipeline() {}

		Pipeline(const std::filesystem::path& shaderSource) : m_shaderModule(shaderSource) {}

		~Pipeline() {}

	protected:
		Shader m_shaderModule;
	};
}