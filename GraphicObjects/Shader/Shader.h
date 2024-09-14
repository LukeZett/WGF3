#pragma once
#include "webgpu/webgpu.h"
#include "Device.h"
#include <iostream>
#include <fstream>
#include <filesystem>
namespace WGF
{
	class RenderPipeline;

	class Shader
	{
		friend RenderPipeline;

		WGPUShaderModule m_shaderModule = nullptr;
		WGPUShaderModuleDescriptor m_desc = {};
		WGPUShaderModuleWGSLDescriptor m_codeDesc = {};
		std::filesystem::path m_source;
	public:
		Shader(Shader&& other) noexcept {
			this->m_shaderModule = other.m_shaderModule;
			this->m_codeDesc = other.m_codeDesc;
			this->m_desc = other.m_desc;
			std::swap(this->m_source, other.m_source);
			other.m_shaderModule = nullptr;
		}

		inline Shader& operator=(Shader&& other) noexcept {
			this->m_shaderModule = other.m_shaderModule;
			this->m_codeDesc = other.m_codeDesc;
			this->m_desc = other.m_desc;
			std::swap(this->m_source, other.m_source);
			other.m_shaderModule = nullptr;
			return *this;
		}

		void SetDesc();

		Shader(const std::filesystem::path& source);

		inline const WGPUShaderModule Get() const { return m_shaderModule; };
		
		Shader();
		
		~Shader();

		void ReloadModule();

		std::string LoadFromPath(const std::filesystem::path& path);
		
		Shader(const Shader&) = delete;
	};
}